# Practica 2 Distribuidos
## Estructura común
- Var global de la id del proceso original, otra para el reloj de Lamport
- Las funciones pueden ser las mismas con un if para diferenciar gracias a la var global de la id del proceso si es el cliente o servidor, o también se pueden usar funciones diferentes 
## Estrura básica P2 o servidor
- Necesitas una struct para guardar la info de los clientes, como sus threads, id y demás.
- Iniciar socket
- Lanzar 2 threads para aceptar a cada cliente
- No hacer join de estos threads hasta más adelante
- Tenemos dos clientes, pero no sabemos quién esquien hasta que ocurre la primera comunicación
- El primer recv del servidor sobre cada cliente irá dirigido a Broadcast, no a un socket específico
- Al hacer los recieve en vez de 1 y 3 le pasamos Broadcast la primera vez
- Ponemos la id de la struct de cada cliente a Broadcast - número de threads de recv mandados y solo hacemos esto en los que la id no ha sido puesta
- En el recv_thread la primera vez que lo ejecutamos por cada cliente (id < Broadcast) debemos hacer join del threads de acept en orden: join(info_clientes[-n_clientes-id].accept_thread)
- En el recv_thread si la id del cliente es de Broadcast ( id < Broadcast) entonces al recibir el mensaje debemos actualizar su id a la id verdadera
- Ahora ya tenemos asignados la id a su correspondiente cliente
- En otra función que se llama después del bucle while correspondiente :Para hacer join de los threads del recv para no tener más de 2 a la vez, al saber la id podemos buscar el info_cliente que corresponde a la id y hacer join de su recv_thread
- Cuando se vuelva ha hacer send o recv se elige el socket usando la id del proceso de destino que se guarda en el struct info_cliente

## Estructura Clientes P1 y P3
- Una variable global para guardar el thread del recv y otra para el socket
- Inicializar el socket y guardarlo en la var global
- Cuando hay que hacer send lo haces usando el socket que hemos guardado
- Para el recv lanzamos el thread guardandolo en la var global y para hacer el join al igual que en el servidor usamos otra función que se llama después de esperar en el programa principal al reloj de lamport
- El socket que se usa en el recv es el que hemos guardado en la var global