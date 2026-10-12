# Minishell

Minishell est un petit shell Unix écrit en C pour reproduire les bases du comportement d'un terminal : lecture de commandes, parsing, expansion des variables, redirections, heredoc, pipes et exécution des built-ins.

## Objectif

Le projet vise à recréer un shell interactif capable de :

- lire une ligne de commande avec `readline`
- gérer les guillemets et l'expansion des variables
- interpréter les redirections et les heredocs
- exécuter des commandes externes et des built-ins
- enchaîner plusieurs commandes avec des pipes
- gérer les signaux du terminal

## Structure

- `srcs/main/` : point d'entrée du programme
- `srcs/lexer/` : découpage de la ligne de commande en tokens
- `srcs/expand/` : expansion des variables et gestion des quotes
- `srcs/parsing/` : transformation des tokens en commandes exécutables
- `srcs/pipeline/` : construction et gestion des pipelines
- `srcs/exec/` : exécution des commandes
- `srcs/redirections/` : gestion des redirections et heredoc
- `srcs/built-in/` : commandes internes comme `cd`, `echo`, `env`, `exit`, `export`, `pwd`, `unset`
- `srcs/signals/` : gestion des signaux
- `srcs/utils/` : fonctions utilitaires et libération mémoire
- `libft/` : bibliothèque personnelle utilisée par le projet
- `includes/` : en-têtes du projet


## Compilation

Depuis la racine du projet :

```bash
make
```

Le `Makefile` compile d'abord `libft`, puis génère l'exécutable `minishell`.

## Commandes Makefile

- `make` : compile le projet
- `make clean` : supprime les objets
- `make fclean` : supprime les objets, `minishell` et la bibliothèque `libft.a`
- `make re` : supprime le projet puis le reconstruit

## Lancement

```bash
./minishell
```

Ensuite, tu peux taper des commandes comme dans un shell classique.

## Exemples

```bash
ls -l
echo hello | wc -c
cat file.txt > out.txt
export USER=test
```

## Fonctionnalités prises en charge

- commandes simples et enchaînement avec `|`
- redirections `>`, `>>`, `<` et heredoc `<<`
- variables d'environnement
- built-ins du shell
- gestion des signaux clavier

## Notes

- Le projet utilise `readline`, donc l'historique et l'édition de ligne sont disponibles.
- La base du shell est celle d'un environnement Unix classique.