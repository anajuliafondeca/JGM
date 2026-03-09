FROM ubuntu:latest

# Instala as dependências necessárias de forma limpa
RUN apt-get update && apt-get install -y \
    g++ \
    libboost-all-dev \
    build-essential \
    libtcmalloc-minimal4 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copia os arquivos
COPY . .

# Comando de compilação corrigido para Linux/Ubuntu
RUN g++ back.cpp -o back.exe -pthread -lboost_system -lboost_filesystem

# Garante que o arquivo tenha permissão de execução
RUN chmod +x back.exe

EXPOSE 18080

CMD ["./back.exe"]