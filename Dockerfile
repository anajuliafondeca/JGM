FROM ubuntu:22.04

# Evita pausas interativas
ENV DEBIAN_FRONTEND=noninteractive

# Passo 1: Instalação das dependências (Adicionando libasio-dev explicitamente)
RUN apt-get update && apt-get install -y \
    g++ \
    libboost-all-dev \
    libasio-dev \
    build-essential \
    mailcap \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Passo 2: Copia os arquivos
COPY . .

# Passo 3: Compilação 
# Adicionei -std=c++17 e mudei a ordem dos links para o linker encontrar tudo
RUN g++ back.cpp -o jogo_exec \
    -std=c++17 \
    -I /usr/include \
    -pthread \
    -lboost_system \
    -lboost_filesystem

RUN chmod +x jogo_exec

EXPOSE 18080

CMD ["./jogo_exec"]