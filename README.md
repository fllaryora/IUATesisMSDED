Modelador de eventos discretos
==============================
BotQueue is a powerful tool for process manager who is responsable to
the desing and tracking process.
The user would  experiment with possible solutions (, i.e. few process's
blueprint, ) for process design of construction/production and then
choose the solution that best suits in cost, production and time
required. Different design solutions may involve different working
sequences and mixtures of resources, which materialize in responses
different system in time, cost and production.


BotQueue es una poderosa herramienta para el gerente de procesos quien es
el responsable del diseño de los procesos de construcción/producción.
El usuario experimentaría con sus posibles soluciónes de diseño del 
proceso de la operación de construcción/producción y escogería la 
solución que mejor se en adapte a costo, producción y tiempo requerido. 
Diferentes soluciones de diseño pueden implicar diferentes secuencias de
trabajo y  mezclas de recursos, que se materializan en respuestas del
sistema diferentes en tiempo, costo y producción.


Se describen los códigos

Para leer archivos con formato json:
Parson ( http://kgabis.github.com/parson/ )
Copyright (c) 2012 Krzysztof Gabis

Se concede permiso, de forma gratuita, a cualquier persona que obtenga
una copia de este software y archivos de documentación asociados, para
tratar el Software sin restricción, incluido sin restricciones los
derechos de usar, copiar, modificar, fusionar, publicar, distribuir, 
sublicenciar y/o vender copias del Software, y para permitir a las 
personas a quienes se proporcione el Software para hacerlo, 
con sujeción a las siguientes condiciones:
 
El aviso de copyright anterior y este aviso de permiso se incluirán en
todas las copias o partes sustanciales del Software.
 
EL SOFTWARE SE ENTREGA "TAL CUAL", SIN GARANTÍA DE NINGÚN TIPO, EXPRESA
O IMPLÍCITA, INCLUYENDO PERO NO LIMITADO A LAS GARANTÍAS DE 
COMERCIALIZACIÓN, IDONEIDAD PARA UN PROPÓSITO PARTICULAR Y NO
INFRACCIÓN. EN NINGÚN CASO, LOS AUTORES O TITULARES DEL COPYRIGHT
( es decir, Krzysztof Gabis) SERÁN RESPONSABLES DE ALGUNA RECLAMACIÓN,
DAÑO U OTRA RESPONSABILIDAD, YA SEA EN UNA ACCIÓN DE CONTRATO, AGRAVIO O
CUALQUIER OTRA FORMA, DERIVADOS DE, O EN CONEXION CON EL SOFTWARE O EL 
USO U OTROS TRATOS EN EL SOFTWARE.

  * parson.c
  * parson.h

Para generar numeros aleatoreos:
El generador de números con distribución uniformes: (1987) George Marsaglia and Arif Zaman
El generador de números con distribución gaussiana: (1992) A.J. Kinderman and J.F. Monahan
El generador de números con distribución exponencial: Wikipedia
El generador de números con distribución triangular: http://www.ntrand.com/triangular-distribution/
El generador de números con distribución beta: Libro "Simulación: Métodos y Aplicaciones" ED: Rama.
El generador de números con distribución lognormal: Wikipedia

  * RNGs.c
  * RNGs.h

Código en construcción...

Para iniciar el programa modelador en MPI:
  * main.c

Para el codigo del scheduler esta en:
  * scheduler.c
  * scheduler.h

Para el codigo del nodo generico esta en:
  * genericNode.c
  * genericNode.h

Done.
