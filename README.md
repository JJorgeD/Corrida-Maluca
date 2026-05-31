# Corrida Maluca 

![Linguagem: C](https://img.shields.io/badge/Linguagem-C-blue.svg)
![Plataformas: Linux | WSL | macOS](https://img.shields.io/badge/Plataforma-Linux%20%7C%20WSL%20%7C%20macOS-yellow.svg)

Jogo criado na linguagem C para a disciplina de Programação Imperativa e Funcional — CESAR School.

O jogador controla um carro em uma pista de 4 faixas e deve desviar dos veículos que surgem à frente. A velocidade aumenta progressivamente com o tempo, tornando o jogo cada vez mais desafiador. O objetivo é percorrer a maior distância possível e bater o high score!

---

##  Demonstração

| Link Screencast |
|----------------|
|  [Link de video no youtube](#) |

---

##  Dependências

- **gcc / build-essential**
- **cli-lib** (incluída no repositório)
- **WSL** (para usuários Windows)

---

##  Instalação e Configuração

###  Linux / WSL (Ubuntu)

#### 1. Instalar ferramentas de compilação

```bash
sudo apt update
sudo apt install build-essential git
```

#### 2. Clonar o repositório

```bash
git clone https://github.com/JJorgeD/Corrida-Maluca.git
cd Corrida-Maluca
```

---

###  macOS

#### 1. Instalar ferramentas de compilação

```bash
xcode-select --install
```

#### 2. Clonar o repositório

```bash
git clone https://github.com/JJorgeD/Corrida-Maluca.git
cd Corrida-Maluca
```

---

##  Compilar o Jogo

###  Usando Makefile 

```bash
make
```

###  Compilação manual

```bash
gcc ./src/*.c main.c -I./include -o corrida
```

---

##  Executar o Jogo

### Usando Makefile

```bash
make run
```

### Diretamente

```bash
./corrida
```

---

##  Como Jogar

| Tecla | Ação |
|-------|------|
| `A` ou `←` | Mover para a esquerda |
| `D` ou `→` | Mover para a direita |
| `Enter` | Sair do jogo |

- Desvie dos carros vermelhos `[X]`
- A velocidade aumenta a cada 30 segundos
- Tente percorrer a maior distância possível
- Ao bater o recorde, digite seu nome para salvar no ranking!

---

##  Estrutura do Projeto