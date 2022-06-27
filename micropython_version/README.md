### Instalar la herramienta para usar el esp32


```bash
$ pip install esptool
```
---
### Instalar el firmware:


- entrar en [esp2 firmware](https://www.micropython.org/download/esp32/)
- descargar el ultimo firmware
- colocar el archivo descargado en el directorio de trabajo
- conectar el ESP a la computadora y buscar el nombre dle puerto. Ej: COM5 en windows.


Si colocas micropython por primera vez:
```bash
$ esptool.py --chip esp32 --port COM5 erase_flash
```

Despues de eso usar (cambiar el nombre del firmware segun sea el caso):
```bash
$ esptool.py --chip esp32 --port COM5 --baud 460800 write_flash -z 0x1000 nombre-archivo-descargado.bin
```
