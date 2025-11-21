# Build script for securOS

# Configuration
$nasm = "nasm"
$qemu = "C:\Program Files\qemu\qemu-system-x86_64.exe"

# Clean
Write-Host "Cleaning up..."
if (Test-Path "bootloader.bin") { Remove-Item "bootloader.bin" }
if (Test-Path "kernel.bin") { Remove-Item "kernel.bin" }
if (Test-Path "securOS.bin") { Remove-Item "securOS.bin" }

# Compile Bootloader
Write-Host "Compiling bootloader..."
& $nasm -f bin src/boot/bootloader.asm -o bootloader.bin
if ($LASTEXITCODE -ne 0) { Write-Error "Bootloader compilation failed"; exit 1 }

# Compile Kernel Entry
Write-Host "Compiling kernel entry..."
& $nasm -f win32 src/kernel/kernel_entry.asm -o kernel_entry.o
if ($LASTEXITCODE -ne 0) { Write-Error "Kernel entry compilation failed"; exit 1 }

# Compile Kernel C
Write-Host "Compiling kernel C..."
gcc -ffreestanding -m32 -c src/kernel/kernel.c -o kernel.o
if ($LASTEXITCODE -ne 0) { Write-Error "Kernel C compilation failed"; exit 1 }

# Link Kernel
Write-Host "Linking kernel..."
ld -m i386pe -o kernel.bin -Ttext 0x1000 kernel_entry.o kernel.o --oformat binary
# Note: 'i386pe' is common for MinGW ld. If using linux cross compiler, use 'elf_i386'.
# If ld fails, we might need to adjust flags based on the specific gcc/ld version installed.
if ($LASTEXITCODE -ne 0) { Write-Error "Linking failed"; exit 1 }

# Combine
Write-Host "Creating disk image..."
cmd /c copy /b bootloader.bin + kernel.bin securOS.bin | Out-Null

# Run
if ($args[0] -eq "run") {
    if (Test-Path $qemu) {
        Write-Host "Running in QEMU..."
        & $qemu securOS.bin
    }
    else {
        Write-Warning "QEMU not found at $qemu. Please check the path."
    }
}

Write-Host "Build complete. Run '.\build.ps1 run' to build and start QEMU."
