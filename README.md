# Chats de mensagens

<h2> O repositorio contem diversos programas clientes e servidores
</h2>

<h3>
TCPEchoSrv
 </h3>

<a>
O arquivo TCPEchoServ contem o codigo de um servidor simples que reenvia a mensagem para o cliente que a enviou
</a>
<br>
<a>
Compilar: gcc -o echoSrv TCPEchoSrv.c <br>
Uso: ./echoSrv 7777
</a>

<h3>
TCPCEchoClient
</h3>

<a>
O arquivo TCPEchoClient contem o codigo de envio de uma mensagem para o servidor
</a>
<br>
<a>
Compilar: gcc -o echoCli TCPEchoClient.c <br>
Uso: ./echoCli 127.0.0.1 7777 ola
