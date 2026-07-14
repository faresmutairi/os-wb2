# MyOS testing evidence

These screenshots were captured from the genuine CSCT Cloud build and QEMU run.

| File | Evidence shown |
|---|---|
| `01-clean-build-and-check.png` | Clean rebuild, required function symbols and Multiboot compliance |
| `02-qemu-kernel-output.png` | `CAFEBABE` pass, mathematics results, positioned framebuffer output and completion message |

The CSCT Cloud host used for testing did not provide `xorriso`. The kernel was
therefore run directly with QEMU after passing `make check`. The repository
still includes the GRUB configuration and ISO-build target.
