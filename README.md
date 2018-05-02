# Requisitos
Os requisitos para compilar e executar o projeto são:
1. CMake vesão 3.1 ou maior.
2. OpenCV versão 3.2.0 ou maior

# Compilar
Para compilar os programas é necessário executar os seguintes comandos:
```console
$ cmake .
$ make
``` 

# Requisito 1
```console
$ ./Requisito1 baseline distancia_focal imagem_esquerda imagem_direita
```
O programa recebe 4 parâmetros, onde:

* baseline é a distancia entre o centro das câmeras
* distancia_focal é a distância focal das câmeras
* imagem_esquerda é a imagem capturada pela câmera da direita
* imagem_direita é a imagem capturada pela câmera da esquerda


Exemplos:
```console
$ ./Requisito1 120 25 Images/Requisito1/aloeL.png Images/Requisito1/aloeR.png
$ ./Requisito1 120 25 Images/Requisito1/babyL.png Images/Requisito1/babyR.png
```


# Requisito 2
Esse requisito foi dividido em duas partes.

## Calibração
```console
$ ./Requisito2_calibracao calibracao_dir
```

O programa recebe 1 parâmetro que é o caminho para o diretorio com as imagens de calibração, o resultado da calibração é salvo no arquivo parameters.xml.

Exemplo:
```console
$ ./Requisito2_calibracao ./Images/Stereo_Calibrate
```

## Calculo do mapa de profundidade
```console
$ ./Requisito2 imagem_esquerda imagem_direita
```

OBS: Não consegui obter bons resultados nesse requisito.

```console
$ ./Requisito2 Images/Stereo_Calibrate/1L.jpg Images/Stereo_Calibrate/1R.jpg
```