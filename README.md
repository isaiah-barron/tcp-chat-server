# Simple TCP Chat Server & Client with Docker

This project implements a simple TCP chat server and client using Docker. The server waits for incoming client connections, and the client sends messages to the server.

### Prerequisites
Make sure you have Docker and Docker Compose installed on your machine.

- [Install Docker](https://docs.docker.com/get-docker/)
- [Install Docker Compose](https://docs.docker.com/compose/install/)

### Project Structure
```
.
├── Dockerfile.server
├── Dockerfile.client
├── docker-compose.yml
├── client_main.cpp
├── client.cpp
├── client.hh
├── server_main.cpp
├── server.cpp
├── server.hh
├── makefile
```

### Steps to Run

#### Step 1: Build the Docker Images

Before running the containers, you need to build the server and client images. In the root of your project directory, run the following command:

```bash
docker-compose build
```

This command will build the Docker images for the server and client based on the Dockerfiles provided.

#### Step 2: Create the Docker Network

Once the images are built, create the Docker network.

```bash
docker network create chatnet
```

#### Step 3: Start the Server Container

Once the Docker network is created, start the server container manually. The server will wait for client connections.

```bash
docker run -it --network chatnet --name tcp_server tcp-chat-server ./server
```

This command starts the server in interactive mode. The `--network chatnet` flag connects the container to the `chatnet` network, which is necessary for communication between the server and client containers.

#### Step 4: Start the Client Container

After the server is running and waiting for connections, you can manually start the client container to connect to the server.

```bash
docker run -it --network chatnet --name tcp_client tcp-chat-client ./client
```

This command starts the client container and connects it to the same `chatnet` network, allowing it to communicate with the server.

### Stopping the Containers

If you need to stop the containers, you can do so with the following commands:

1. To stop the server container:
   ```bash
   docker stop tcp_server
   ```

2. To stop the client container:
   ```bash
   docker stop tcp_client
   ```

3. To remove the stopped containers:
   ```bash
   docker rm tcp_server tcp_client
   ```

#### Optional: Restart Containers

If you'd like to restart the containers, use the following commands:

1. Restart the server:
   ```bash
   docker start tcp_server
   ```

2. Restart the client:
   ```bash
   docker start tcp_client
   ```

### Troubleshooting

- **Server isn't starting fast enough**: The server may take a while to start. Make sure the server is fully up and running before starting the client container.
- **Client can't connect**: Ensure both containers are connected to the same network and the server is listening on the correct port.

---

Let me know if you need any further adjustments!
