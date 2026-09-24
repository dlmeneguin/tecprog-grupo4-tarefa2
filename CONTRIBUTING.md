# Contribuindo para o projeto

Este documento explica como contribuir com o projeto usando Git e GitHub, mesmo que você nunca tenha usado Git antes.

## 1. Pré-requisitos

Você precisa ter:

* [Git](https://git-scm.com/) instalado;
* Uma conta no GitHub;
* Acesso ao repositório.

Para verificar se o Git está instalado, abra o terminal e execute:

```bash
git --version
```

---

# 2. Clonando o projeto

Primeiro, copie a URL do repositório no GitHub.

Depois, no terminal, execute:

```bash
git clone URL_DO_REPOSITORIO
```

Por exemplo:

```bash
git clone https://github.com/usuario/projeto.git
```

Entre na pasta do projeto:

```bash
cd projeto
```

Agora você possui uma cópia do projeto no seu computador.

---

# 3. Criando uma branch

**Nunca trabalhe diretamente na branch `main`.**

Antes de começar uma alteração, crie uma nova branch:

```bash
git checkout -b nome-da-branch
```

Por exemplo:

```bash
git checkout -b feature/login
```

## Como nomear a branch?

O nome deve deixar claro o que você está fazendo.

Use:

```text
tipo/descricao
```

Alguns exemplos:

```text
feature/sistema-de-login
feature/pagina-de-ranking
fix/erro-no-cronometro
fix/cor-do-botao
refactor/organizar-codigo
docs/atualizar-readme
```

### Tipos mais comuns

* `feature/` → criação de uma funcionalidade nova;
* `fix/` → correção de um problema;
* `refactor/` → alteração na estrutura do código sem mudar seu funcionamento;
* `docs/` → documentação.

### Regras para o nome

Prefira:

```text
feature/sistema-de-login
```

Em vez de:

```text
MinhaBranchNova
```

Evite espaços, acentos e nomes muito genéricos.

---

# 4. Trabalhando no projeto

Agora você pode modificar os arquivos normalmente.

Você pode fazer quantas alterações quiser antes de criar um commit.

Para verificar o que foi alterado:

```bash
git status
```

---

# 5. E se alguém atualizou o projeto?

Imagine que você começou a trabalhar e outra pessoa adicionou código novo ao repositório.

Nesse caso, é uma boa ideia atualizar sua branch antes de continuar.

Primeiro, verifique se você possui alterações que ainda não foram commitadas:

```bash
git status
```

Se você **não possui alterações**, pode simplesmente atualizar a `main`:

```bash
git checkout main
git pull
```

Depois volte para sua branch:

```bash
git checkout nome-da-sua-branch
```

E traga as alterações da `main` para ela:

```bash
git merge main
```

---

# 6. E se eu tiver alterações que ainda não quero commitar?

Nesse caso, você pode usar o **stash**.

O `stash` guarda temporariamente suas alterações, permitindo que você troque de branch ou atualize o código sem precisar fazer um commit ainda.

Execute:

```bash
git stash
```

Suas alterações serão guardadas temporariamente e seu código ficará como estava antes das alterações.

Agora você pode atualizar a `main`:

```bash
git checkout main
git pull
```

Volte para sua branch:

```bash
git checkout nome-da-sua-branch
```

E atualize sua branch:

```bash
git merge main
```

Depois, recupere suas alterações:

```bash
git stash pop
```

O fluxo completo fica:

```bash
git stash

git checkout main
git pull

git checkout nome-da-sua-branch
git merge main

git stash pop
```

### O que cada comando faz?

* `git stash` → guarda temporariamente suas alterações;
* `git pull` → baixa e aplica as alterações mais recentes do repositório;
* `git merge main` → traz as alterações da `main` para sua branch;
* `git stash pop` → recupera as alterações que estavam guardadas.

> **Atenção:** pode acontecer de surgir um conflito ao executar `merge` ou `stash pop`. Se isso acontecer, não se preocupe. O Git indicará quais arquivos possuem conflito. Resolva os conflitos manualmente e depois continue o processo.

---

# 7. Adicionando as alterações

Quando terminar seu trabalho, veja novamente o que foi alterado:

```bash
git status
```

Para adicionar os arquivos ao próximo commit:

```bash
git add .
```

O `.` significa "adicionar todas as alterações".

Também é possível adicionar um arquivo específico:

```bash
git add arquivo.py
```

---

# 8. Fazendo o commit

Depois de adicionar os arquivos, crie um commit:

```bash
git commit -m "Descrição da alteração"
```

Por exemplo:

```bash
git commit -m "Adiciona sistema de login"
```

O commit é como um **registro/snapshot das suas alterações**.

Prefira mensagens que expliquem o que foi feito:

```text
Adiciona sistema de ranking
Corrige erro no cronômetro
Adiciona página de perfil
Atualiza documentação
```

Evite mensagens muito vagas:

```text
mudanças
alterações
coisas
teste
```

---

# 9. Enviando sua branch para o GitHub

**Antes de executar o `push`, confira em qual branch você está:**

```bash
git branch
```

A branch atual aparecerá com um `*`.

Por exemplo:

```text
  main
* feature/sistema-de-login
```

Se estiver na branch correta, envie suas alterações:

```bash
git push origin nome-da-sua-branch
```

Por exemplo:

```bash
git push origin feature/sistema-de-login
```

### ⚠️ Importante

Não faça:

```bash
git push origin main
```

a menos que você tenha sido orientado especificamente a fazer isso.

Suas alterações devem ser enviadas para **a sua branch**.

---

# 10. Criando um Pull Request

Depois do `push`, acesse o repositório no GitHub.

O GitHub normalmente mostrará uma opção para criar um **Pull Request (PR)**.

O Pull Request serve para pedir que suas alterações sejam revisadas e, caso estejam corretas, adicionadas à `main`.

No Pull Request:

1. Confira se a branch de destino é `main`;
2. Confira se a branch de origem é a sua branch;
3. Escreva um título que explique a alteração;
4. Explique brevemente o que foi feito;
5. Envie o Pull Request.

Por exemplo:

```text
main ← feature/sistema-de-login
```

Isso significa:

> Quero adicionar as alterações da `feature/sistema-de-login` à `main`.

---

# 11. Fluxo completo

Na maioria das vezes, seu trabalho seguirá este fluxo:

### Primeira vez no projeto

```bash
git clone URL_DO_REPOSITORIO
cd projeto

git checkout -b feature/minha-feature
```

Faça suas alterações.

Depois:

```bash
git add .
git commit -m "Adiciona minha feature"
git push origin feature/minha-feature
```

Depois disso, abra um **Pull Request** no GitHub.

---

## Se o projeto tiver recebido alterações enquanto você trabalha

Se você **não possui alterações não commitadas**:

```bash
git checkout main
git pull

git checkout feature/minha-feature
git merge main
```

Continue trabalhando.

Se você **possui alterações não commitadas**:

```bash
git stash

git checkout main
git pull

git checkout feature/minha-feature
git merge main

git stash pop
```

Depois:

```bash
git add .
git commit -m "Atualiza minha feature"
git push origin feature/minha-feature
```

E então abra ou atualize o Pull Request.

---

# 12. Resumo dos principais comandos

| Comando           | O que faz                                      |
| ----------------- | ---------------------------------------------- |
| `git clone`       | Baixa o projeto para o computador              |
| `git checkout -b` | Cria uma nova branch e muda para ela           |
| `git status`      | Mostra o estado das alterações                 |
| `git branch`      | Mostra as branches                             |
| `git stash`       | Guarda alterações temporariamente              |
| `git stash pop`   | Recupera alterações guardadas                  |
| `git pull`        | Atualiza uma branch com o repositório remoto   |
| `git merge`       | Junta as alterações de outra branch            |
| `git add`         | Seleciona alterações para o commit             |
| `git commit`      | Registra as alterações                         |
| `git push`        | Envia os commits para o GitHub                 |
| Pull Request      | Solicita a revisão e integração das alterações |

---

# 13. Regra principal

O fluxo que você deve ter em mente é:

```text
main
 │
 ├──→ criar sua branch
 │
 │    trabalhar
 │
 │    git add
 │    git commit
 │    git push
 │
 └──→ Pull Request
          │
          ↓
         main
```

**Não trabalhe diretamente na `main`.**

Cada funcionalidade ou correção deve ser feita em uma branch própria e enviada através de um Pull Request.
