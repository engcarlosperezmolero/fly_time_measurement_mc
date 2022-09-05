## Dentro del loop del programa de arduino
---
Lo primero que se verifica dentro del loop es si la variable ```comenzo``` tiene el valor de "si" o de "no", esta variable es actualizada por el cliente, cuando es presionado el boton de **Comenzar Serie** de la interfaz del administrador se envia mediante websockets una orden que es procesada en la funcion ```onWebSocketEvent``` cuando se recibe un texto.


Las acciones dentro del loop se realizan cada cierto tiempo, este tiempo esta establecido dentro de la variable ```tiempoEspera``` que es de 5 milisegundos.


### Variable ```comenzo``` con valor "no"
---
Mientras no se haya comenzado la serie de trampolin (No se ha presionado el boton en el cliente administrador) se realiza una verificacion de si el gimnasta se encuentra en el aire o en la lona. Para este fin se monitorea el valor de las variables ```infrarrojo``` y ```estadoAnterior```.

El objetivo es que mientras el gimnasta este en el aire el Administrador (persona que controla el cliente administrador) pueda presionar el boton y cambiar el valor de la variable ```comenzo``` a "si".


### Variable ```comenzo``` con valor "si"
---
Cuando ya se comenzo la serie de trampolin se van a seguir monitoreando los valores de ```infrarrojo``` y ```estadoAnterior```, o dicho de otra manera si el gimnasta esta en el aire o en la lona, al chequear el ```estadoAnterior``` se puede conocer el momento en que el gimnasta pasa de estar en la lona a estar en el aire y viceversa.

Ademas algo importante es que se esta comprobando tambien el numero de saltos que el gimnasta ha realizado, en el caso de que haya realizado los 10 saltos se dejara de realizar el proceso de medicion de tiempo de vuelo.

En caso de que ```infrarrojo``` tenga el valor de 0 querra decir que el gimnasta esta en la lona. En este caso se van a realizar las diferencias del tiempoDespegue con el tiempoAterrizaje. En caso de que este en el salto 1 el tiempoVuelo es calculado con el tiempoDespegueInicial (el cual se obtiene su valor en la ultima vez que el gimnasta aterrizo antes de que se comenzara la serie). Algo que tambien sucede cada vez que el gimnasta haya aterrizado es enviar datos al cliente mediante websockets dentro de la variable json ```object``` y se aumentaa el contador de saltos.

En caso de que ```infrarrojo``` tenga el valor de 1 querra decir que el gimnasta esta en el aire, en este momento simplemente se media el tiempoDespegue.