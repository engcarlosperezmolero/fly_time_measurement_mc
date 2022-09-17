## Dentro del loop del programa de arduino
---
Lo primero que se verifica dentro del loop es si la variable ```comenzo``` tiene el valor de "si" o de "no", esta variable es actualizada por el cliente, cuando es presionado el boton de **Comenzar Serie** de la interfaz del administrador se envia mediante websockets una orden que es procesada en la funcion ```onWebSocketEvent``` cuando se recibe un texto.


Las acciones dentro del loop se realizan cada cierto tiempo, este tiempo esta establecido dentro de la variable ```tiempoEspera``` que es de 5 milisegundos.


#### Variable ```comenzo``` con valor "no"
---
Mientras no se haya comenzado la serie de trampolin (No se ha presionado el boton en el cliente administrador) se realiza una verificacion de si el gimnasta se encuentra en el aire o en la lona. Para este fin se monitorea el valor de las variables ```infrarrojo``` y ```estadoAnterior```.

El objetivo es que mientras el gimnasta este en el aire el Administrador (persona que controla el cliente administrador) pueda presionar el boton y cambiar el valor de la variable ```comenzo``` a "si".


#### Variable ```comenzo``` con valor "si"
---
Cuando ya se comenzo la serie de trampolin se van a seguir monitoreando los valores de ```infrarrojo``` y ```estadoAnterior```, o dicho de otra manera si el gimnasta esta en el aire o en la lona, al chequear el ```estadoAnterior``` se puede conocer el momento en que el gimnasta pasa de estar en la lona a estar en el aire y viceversa.

Ademas algo importante es que se esta comprobando tambien el numero de saltos que el gimnasta ha realizado, en el caso de que haya realizado los 10 saltos se dejara de realizar el proceso de medicion de tiempo de vuelo.

En caso de que ```infrarrojo``` tenga el valor de 0 querra decir que el gimnasta esta en la lona. En este caso se van a realizar las diferencias del tiempoDespegue con el tiempoAterrizaje. En caso de que este en el salto 1 el tiempoVuelo es calculado con el tiempoDespegueInicial (el cual se obtiene su valor en la ultima vez que el gimnasta aterrizo antes de que se comenzara la serie). Algo que tambien sucede cada vez que el gimnasta haya aterrizado es enviar datos al cliente mediante websockets dentro de la variable json ```object``` y se aumentaa el contador de saltos.

En caso de que ```infrarrojo``` tenga el valor de 1 querra decir que el gimnasta esta en el aire, en este momento simplemente se media el tiempoDespegue.


## Que sucede cuando llegan señales desde el cliente
---

La comunicación con el cliente es mantenida a traves del protocolo web sockets, el servidor esta constantemente chequeando el estado de la conexion asi como escuchando los distintos mensajes que el cliente pueda enviar.

Para enviar el mensaje se usa formato JSON el cual es deserializado cuando entra al servidor de parte del cliente.

La forma del mensaje que envia el cliente es:

```json
{
    "reiniciar": "si", -> si, no
    "descargar_respaldo": "si", -> si, no
    "comenzo_serie": "si", -> si, no
    "nombre": "",
    "nivel": "",
    "categoria": ""
}
```

La forma del mensaje que enviar el servidor a todos los clientes es:

```json
{
    "estado_serie": "comenzo", -> comenzo, no_comenzo
    "tiempo": "no_disponible", -> no_disponible, respaldo, reiniciar
    "respaldo_tiempos": "no_solicitado", -> '10,20', no_solicitado, reinicar 
    "reiniciar": "no", -> si, no
    "info_gimnasta": "Nombre, Nivel, Categoria"
}
```

## Si no hay error
Cuando no hay error se realiza la asignación de los valores de JSON a las variables respectivas y se proceden a verificar varias cosas.

## Si comenzo serie
Si comenzo la serie el contador de saltos se le suma 1 y se envia el siguiente mensaje a los clientes:

```json
{
    "estado_serie": "comenzo",
    "tiempo": "no_disponible",
    "respaldo_tiempos": "no_solicitado",
    "reiniciar": "no",
    "info_gimnasta": "Nombre, Nivel, Categoria"
}
```

## Si se descarga respaldo
Se arma una string en donde usando el array de tiempos[10] se colocan los numeros separados por comas y se envia el siguiente JSON a todos los clientes:

```json
{
    "estado_serie": "no_comenzo",
    "tiempo": "respaldo",
    "respaldo_tiempos": "2.132,2.321,2.031,2.301,1.098....",
    "reiniciar": "no",
    "info_gimnasta": ""
}
```

## Si se reinicia
Distintas variables dentro del Servidor vuelven a sus estados iniciales:
- estadoAnterior a 1
- contadorSaltos a 0
- comenzo a "no"
- tiempoPrevio a 0
- json_string a ""

Luego el JSON que enviar a los clientes es:

```json
{
    "estado_serie": "reiniciar",
    "tiempo": "reiniciar",
    "respaldo_tiempos": "reiniciar",
    "reiniciar": "si",
    "info_gimnasta": ""
}
```

## Endpoints del servidor
---

### El dominio
El dominio de la RED creada por el dispositivo ESP32 es:

http://192.168.4.1:8097

### GET ```/admin-le-tramp```

Accede a la pagina del administrador en donde se encuentran los controles y botones de la serie.

Url completa es http://192.168.4.1:8097/admin-le-tramp

### GET ```/```

Accede a la pagina del observador en donde un usuario puede ver en tiempo real la medicion de tiempo de vuelo del gimnasta.

Url completa es http://192.168.4.1:8097/
### GET Not Found

Cualquier otro endpoint solo devolvera "Not Found"