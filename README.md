# checker-ai-program-in-c

This is a program of a checker AI written in C ,which is an  Tongji University's Software Engineering assignment of the first semester in the first year of the university.Using an 8-level alpha-beta pruning algorithm,this AI can  artifically battle with users in checker:

Input rules:

1.START request:

At the beginning of the game users should input START request with number 1 or 2 to indicate the side of the ai:

```
START 1
```

   the AI will be selected as black side 



```
START 2
```

   the AI will be selected as white side 





after input, an current chessboard map will be shown to the users











2.PLACE request:

when it's users turn to place draughts,input PLACE with :1.a number of the blocks that the chess  goes through;2.the coordinates of each block that the chess  goes through.

For instance	

```
PLACE 2 2,1 3,2
```

​      means you move a white chess from (2,1) to (3,2),going through 2 blocks.



After input, an current chessboard map will be shown to the users:



when users can ead enemy chess,input:

```
 PLACE 3 3,2 5,4 3,6
```

​	in this way the white chess can eat two black chess in block (4,3) and (4,5),going through 3 blocks:









3. TURN request:

   when it's AI turn to place a chess, simply input TURN to let AI decide which step to place.

   ```
   TURN
   ```

   After input,a current board wil be shown to the users ,with the used time of the calculation and the information of the movement of AI ,similar to the information users input in PLACE request:

   

   

   



