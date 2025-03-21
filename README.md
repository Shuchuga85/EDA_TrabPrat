
# Trabalho Prático

Licenciatura em Engenharia de Sistemas Informáticos 2024-25

Estrutura de Dados Avançados 

## Aluno

| Número | Nome |
| -----   | ---- |
| 31521     | Vitor Rezende  |


## Organização

[src/](./src/)  Código da solução desenvolvida <br>
[doxdoc/](./doxdoc/)  Documentação com o relatórioi gerado pela ferramenta Doxygen <br>
[doc/](./doc/)  Documentação com o relatório

## Executar

```[bash]
cd ./src
make run
```

## Compilar o código fonte

```[bash]
cd ./src
make all
```

## Gerar documentação do código

```[bash]
doxygen ./Doxyfile
```

## Gerar Relatório com Latex
Para gerar o ficheiro 'main.pdf' executar:

```[bash]
cd ./doc
pdflatex main.tex
```