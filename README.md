Projeto de jogo com gráficos 3D desenvolvido para a disciplina de Computer Graphics na UFABC.

### Aluno

Vinicius Nunes Zorzetti
11068214

### Descrição do jogo

O jogo é um labirinto em 3D no qual o jogador começa de um lado do labirinto e tem que chegar do outro lado para vencer.

Abaixo tem a imagem do labirinto, onde em azul é um dos caminhos para sair do labiritinto. (No jogo, considera-se que o usuário está na parte de baixo do labirinto e tem que chegar no topo).

![alt Labirinto](public/maze.png "Maze")

### Controles

Seta para frente ou W = Move para frente

Seta para tráz ou S = Move para trás

Seta para esquerda ou A = Vira para esquerda

Seta para a direita ou D = Vira para a direita

Q = Anda lateralmente para esquerda

E = Anda lateralmente para a direita

### Descrição da implementação

O jogo foi baseado em diversas implementações desenvolvidas na disciplina.

Utilizei primeiramente como base, a implementação de "LookAt", onde aproveitei o sistema de funcionamento da câmera (que representaria o personagem andando pelo labirinto), o mecanismo de projeção em perspectiva e o chão (ground).

O labirinto é representado por uma grade 19x19, onde cada posição é espaçada por 1.0f unidades.

Em cada célula dessa grade do labirinto, pode conter ou não um bloco retangular vertical, representando um trecho de parede do labirinto. Esse bloco foi representado pelo objeto box.obj da implementação "Starfield".

Quanto o usuário se move, ele altera a posição da câmera. Já os blocos das paredes do labirinto permanecem fixas.

A colisão com uma parede é detectada a partir da distância entre a posição da câmera e a posição do centro do bloco de parede, quando essa distância é menor que 0.71f, consideramos que há uma colisão. (Esse valor é a distância do centro de um bloco 1x1 até um vértice de sua extremidade). Para o cálculo dessa distância, considerou-se as posições das projeções da posição da câmera e da parede no chão.

O sistema de colisão foi implementado nas funções de movimentação da câmera, antes de movimentar a câmera, verifica-se se a nova posição seria uma posição de colisão, se for, não faz nada, caso contrário, realiza o movimento.

Como a forma utilizada para criar o labirinto implica na instanciação de diversos blocos de parede, e cada bloco precisaria armazenar sua posição para poder calcular a distância da câmera e do objeto, e também cada instancia ter sua própria matriz modelo, portanto, foi aproveitada a implementação de "Starfield".

O controle do jogo, para exibir que o usuário venceu, utilizou-se como base a implementação de "Asteroids".

## Informações adicionais

Jogo desenvolvido usando o framework ABCg. Mais informações abaixo.

## ABCg

======

Development framework accompanying the course MCTA008-17 Computer Graphics at [UFABC](https://www.ufabc.edu.br/).

---

### Credits

ABCg was developed by Harlen Batagelo.

### License

ABCg is licensed under the MIT License. See [LICENSE](https://github.com/hbatagelo/abcg/blob/main/LICENSE) for more information.
