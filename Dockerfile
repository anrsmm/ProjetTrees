FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    make \
    minisat \
    python3 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

# On compile les executables principaux utilises en demo.
RUN make resolutionTents grille-cnf minisat-grille cnf-3sat

# Commande par defaut: resolution complete sur une petite instance.
CMD ["./resolutionTents", "Tests/Grille1_66.txt"]
