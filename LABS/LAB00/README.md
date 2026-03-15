### Descripción del Proyecto
Implementación de un juego básico en C++ en Unreal Engine 4.27.2. El sistema cuenta con un peón controlado por el jugador (ALAB00Pawn) y una clase enemigo AEnemy que patrulla el mapa y ataca al jugador.

## Clases y Objetos
- *ALAB00Pawn*: Representa al jugador. Hereda de APawn y actúa como la entidad controlable por el usuario.ç
- *AEnemy*: Hereda de AActor. Se encargan de la navegación por puntos de ruta y el ataque automático.
- *ALAB00Projectile*: Representa las balas disparadas por el jugador y los enemigos. Gestiona su propia colisión y ciclo de vida.
- *ALAB00GameMode*: La clase administrativa que define las reglas del juego.

## Métodos
- *BeginPlay()*: Método de inicialización donde los enemigos calculan sus rutas aleatorias y el jugador configura su estado inicial.
- *Tick(float DeltaTime)*: Procesa las acciones cuadro por cuadro.
- *FireShot(FVector FireDirection)*: Método en el peón que instancia proyectiles basados en la entrada del jugador.
- *ShootAtPlayer()*: Método del enemigo que calcula el vector hacia la posición del jugador y dispara un proyectil cada 2 segundos.
- *OnHit()*: Método que gestiona la respuesta física cuando un proyectil colisiona con otro objeto.

## Propiedades
- *MovementSpeed*: Determinan la rapidez de desplazamiento de las entidades.
- *Waypoints*: Arreglo de vectores que almacena las posiciones de patrulla del enemigo.
- *FireRate*: Define la cadencia de disparo (0.1s para el jugador, 2s para el enemigo).
- *GunOffset* / *SpawnLocation* (en ShootAtPlayer()): Vector que define el punto desde donde sale el proyectil para evitar colisiones internas.

## Lógica del juego
*1 Control del jugador*: El peón utiliza el sistema de entrada de Unreal para moverse en los ejes X e Y, orientádnose y disparando proyectiles en la dirección indicada por las teclas de dirección.
*2 Enemigos*:
	- Patrulla: Al iniciar, el enemigo genera 10 puntos aleatorios dentro de los límites definidos y se desplaza hacia ellos secuencialmente.
	- Ataque automático: Mediante un FTimerHandle, el enemigo detecta la posición del peón del jugador y crea un proyectil dirigido al actor.
*3 Sistema de proyectiles*: Los proyectiles se desplazan a una velocidad constante y tienen un tiempo de vida limitado para optimizar el rendimiento en memoria.
