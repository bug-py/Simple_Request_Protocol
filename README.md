# Simple Request Protocol 
- ## Présentation
#### SRP est une bibliothèque écrite en C qui implémente un protocole personnalisé basé sur TCP  
#### Comme vous le savez, TCP fonctionne avec un flux d'octets. 
#### Ce protocole découpe ce flux en requêtes pour le client et réponse pour le serveur 
#### Donc, en principe, le client envoie une "request" au serveur et celui-ci répond avec une "response"
- ## Installation
#### Ce projet fonctionne sur LINUX avec le compilateur gcc 13.3.0
#### afin de compiler la lib statique, il faut utiliser cette commande avec MAKE
```bash
make all
```
#### ou encore si vous voulez compiler avec les tests 
```bash
make test
```
- ## Structure du Protocol
#### Afin de mieux comprendre le protocole, il faut vous exposer la structure interne  de cette lib
```c
typedef struct{
    status_t status;
    uint16_t id;
    uint32_t length;
    char* body; 
}message_t; 
```
#### Cette struct constitue le "packet" dans lequel seront encapsulées les données qui seront transmises dans le réseau 
#### Cette lib utilise la même structure pour les requêtes et réponses
#### statuts : est une enum qui décrit l'état de la requête ou de la réponse
#### id : sert au client, elle désigne la ressource demandé au serveur
#### length : est le nombre d'octets que prend le message principal
#### body : est le message à envoyer
#### *Vous pouvez retrouver les détails du protocole dans [protocol.h](/include/internal/protocol.h)*
- ## Utilisation
#### Dans cette partie, on va écrire un petit client et serveur avec ce protocole
#### Donc, avec le serveur qui tournera en boucle et traitera les requêtes
#### l'utilisateur pourra lancer le client 
#### Avec lequel il enverra grâce au terminal des caractères que le serveur convertira soit en majuscule soit en minuscule
### Client 
![client terminal](/img/client_terminal.png)
### Serveur
![serveur termianl](/img/server_terminal.png)

