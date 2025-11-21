#include "ramdisk.h"
#include "../crypto/sha512.h"
#include "../utils/string.h"
#include "../utils/logger.h"

// The actual ramdisk storage
static uint8_t ramdisk_storage[RAMDISK_SIZE];

// The 128-bit key used for encryption
static uint8_t ramdisk_key[16];

// Initialization Vector (Fixed for demo, should be random)
static const uint8_t ramdisk_iv[8] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xCA, 0xFE, 0xBA, 0xBE };

void ramdisk_init(const uint8_t* key) {
    memcpy(ramdisk_key, key, 16);
    
    // Initialize with some dummy encrypted data for the demo
    // We'll write "This is a secret message!" at offset 0
    // But we have to write it ENCRYPTED.
    // Since our cipher is symmetric (XOR), we can just use ramdisk_write to set it up.
    
    // Clear ramdisk
    memset(ramdisk_storage, 0, RAMDISK_SIZE);
    
    log_info("Ramdisk initialized (Encrypted)");
}

// Helper to apply the stream cipher
static void ramdisk_crypt(size_t offset, size_t size, uint8_t* buffer, int write) {
    uint8_t keystream[SHA512_DIGEST_SIZE];
    size_t current_offset = offset;
    size_t bytes_processed = 0;
    
    while (bytes_processed < size) {
        // Calculate which block we are in
        uint64_t block_index = current_offset / SHA512_DIGEST_SIZE;
        size_t block_offset = current_offset % SHA512_DIGEST_SIZE;
        
        // Generate keystream for this block
        triple_sha512_sc_block(ramdisk_key, ramdisk_iv, block_index, keystream);
        
        // XOR data with keystream
        size_t bytes_to_process = SHA512_DIGEST_SIZE - block_offset;
        if (bytes_to_process > (size - bytes_processed)) {
            bytes_to_process = size - bytes_processed;
        }
        
        for (size_t i = 0; i < bytes_to_process; i++) {
            if (write) {
                // Encrypt: Storage = Buffer ^ Keystream
                ramdisk_storage[current_offset + i] = buffer[bytes_processed + i] ^ keystream[block_offset + i];
            } else {
                // Decrypt: Buffer = Storage ^ Keystream
                buffer[bytes_processed + i] = ramdisk_storage[current_offset + i] ^ keystream[block_offset + i];
            }
        }
        
        current_offset += bytes_to_process;
        bytes_processed += bytes_to_process;
    }
}

void ramdisk_read(size_t offset, size_t size, uint8_t* buffer) {
    if (offset + size > RAMDISK_SIZE) {
        log_error("Ramdisk read out of bounds");
        return;
    }
    ramdisk_crypt(offset, size, buffer, 0);
}

void ramdisk_write(size_t offset, size_t size, const uint8_t* buffer) {
    if (offset + size > RAMDISK_SIZE) {
        log_error("Ramdisk write out of bounds");
        return;
    }
    // Cast const away because we're just reading from it, but our helper is generic
    ramdisk_crypt(offset, size, (uint8_t*)buffer, 1);
}

const uint8_t* ramdisk_get_raw_pointer() {
    return ramdisk_storage;
}
