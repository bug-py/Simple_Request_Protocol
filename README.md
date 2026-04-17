# Simple Request Protocol 
- ## Présentation
#### SRP est une bibliothèque écrite en C qui implémente un protocole personnalisé basé sur TCP  
#### Comme vous le savez, TCP fonctionne avec un flux d'octets. 
#### Ce protocole découpe ce flux en requêtes pour le client et réponses pour le serveur 
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
- ## Structure du Protocole
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
#### id : sert au client, elle désigne la ressource demandée au serveur
#### length : est le nombre d'octets que prend le message principal
#### body : est le message à envoyer
#### Enfin pour ce qui en est de l'allocation 
#### Il est toujours préférable d'allouer *body* sur la heap et utiliser la fonction *free_message()* pour le libérer 
```c
void free_message(message_t* message);
```
#### *Vous pouvez retrouver les détails du protocole dans [protocol.h](/include/internal/protocol.h)*
- ## Utilisation
#### Donc cette lib expose les fonctions pour gérer le client et le serveur dans les fichiers headers se trouvant dans le dossier [api](/include/api)
### Client
#### *Tous les prototypes de fonction se trouve dans [client.h](/include/internal/client.h)*
### CreateConnection()
```c
int CreateConnection(connection_t* connection,char* ip,uint16_t port);
```
#### ***Usage*** :
#### Initialise la structure interne et convertis l'IP et le port dans un format compréhensible par le réseau 
#### Il n'ouvre aucune connexion
#### ***Arguments*** :
- #### connection :
#### il pointe vers une struct interne connection_t qu'il faudra allouer sur la heap ou la stack 
#### Celui-ci représente la connexion
- #### ip : 
#### c'est l'IPV4 de l'ordinateur distant
- #### port : 
#### le port de l'ordinateur distant
#### ***Valeur de retour*** :
#### En cas de succès la fonction retourne 0 sinon -1 avec une IP invalide
### Request()
```c
state_request_t Request(connection_t* connection,message_t* request,message_t* response,bool close_connection);
```
#### ***Usage*** : 
#### Ouvre et ferme la connexion et procède à l'envoi de la requête au serveur et la réception de la réponse
####  ***Arguments*** :
- #### connection : 
#### il pointe vers une struct interne qu'il faudra s'assurer  d'initialiser avec CreateConnection()
- #### request : 
#### un ptr vers une struct [message_t](include/internal/protocol.h) qu'il faudra allouer et remplir elle constitue le message envoyé au serveur
- #### response : 
#### un ptr vers une struct  [message_t](include/internal/protocol.h) qu'il faudra allouer elle accueillera la réponse du serveur
- #### close_connection :  
#### *false* : elle ferme la connexion après la requête  
#### *true* : elle laisse ouverte la connexion pour une persistance
#### A la fin dans le cas d'une connexion persistante ne pas oublier de faire cette appel
```c
Request(conn,NULL,NULL,true);
```
#### Afin de fermer la connexion 
#### ***Valeur de retour*** :
#### Cette fonction retourne l'état de la requête avec l'enum state_request_t qui prend trois valeurs
- #### CONNECTION_ERROR
#### Le client n'a pas réussi à établir une connexion avec l'ordinateur distant cible
- #### REQUEST_FAILED
#### La client a réussi à établir une  connexion mais lors de l'échange d'informations une erreur s'est produite (fermeture brutale,format de la réponse incorrect) 
#### La connexion est fermée automatiquement
- #### REQUEST_SUCCESS
#### La requête a réussi la connexion reste ouverte selon *close_connection*
#### *response* contient le message du serveur qu'il faudra ne pas oublier de désallouer de la heap avec *free_message()*
- ## Test 
#### Les fichiers test présents dans ce projet implémentent un petit service avec ce protocole
#### Donc, avec le serveur qui tournera en boucle et traitera les requêtes
#### l'utilisateur pourra lancer le client 
#### Avec lequel il enverra grâce au terminal des caractères que le serveur convertira soit en majuscule soit en minuscule
### Client 
![client terminal](/img/client_terminal.png)
### Serveur
![serveur termial](/img/server_terminal.png)

