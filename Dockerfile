FROM ubuntu:22.04
RUN apt-get update && apt-get install -y g++ libboost-all-dev build-essential mailcap && rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY . .
RUN g++ back.cpp -o jogo_exec -pthread -lboost_system -lboost_filesystem
RUN chmod +x jogo_exec
EXPOSE 18080
CMD ["./jogo_exec"]