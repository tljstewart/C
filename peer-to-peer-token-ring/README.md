TO COMPILE
`make`

TO START SERVER
`./bbserver <ServerPort> <NumberOfClients>`

TO START CLIENT
`./bbclient [-new] <ClientPort> <OthersPort> <sharedFile>`

-new is only needed when connected to the server. When joining an exsisting ring, -new is not needed.

TO USE CLIENT AFTER STARTING CLIENT
0. Appends a new message.
1. Read a message.
2. Displays a range of messages.
3. Leave the token ring.
