# Projeto-Sistemas-Operacionais
Projeto de Aplicação que utilize conceitos apresentados pela matéria de Sistemas Operacionais
- Linguagem C
- Orientador: João Carlos de Moraes Morselli Junior

## Calculadora de equações matemáticas

### Bhaskara
- Utilizando pipes, faz com que um processo Pai recolha os dados e comunique para o processo Filho para realização da determinante delta e das raízes, se for necessário, após isso estes valores retornam para o processo Pai

### Pitágoras
- Utilizando Threads da biblioteca pthread.h. De forma intercalada, faz o cálculo de um cateto e hipotenusa com valores que são recolhido pelas threads. A região crítica é controlada por uma variável semáforo "mutex" para evitar o acesso simultâneo das threads.
