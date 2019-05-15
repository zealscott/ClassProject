/* animal.pro
  animal identification game.  

    start with ?- run.     */

run :- animal_is(Animal),
      write('I guess that the animal is: '),
      write(Animal),
      nl,
      undo.

/* hypotheses to be tested */
/* 是否为企鹅 */
animal_is(penguin)   :- penguin, !.
/* 是否为金钱豹 */
animal_is(leopard)   :- leopard, !.
/* 是否为猪 */
animal_is(pig)   :- pig, !.
/* 是否为长颈鹿 */
animal_is(giraffe)   :- giraffe, !.
/* 是否为鸵鸟 */
animal_is(ostrich)   :- ostrich, !.
/* 是否为斑马 */
animal_is(zebra)     :- zebra, !.
/* 是否为老虎 */
animal_is(tiger)     :- tiger, !.
/* 是否为信天翁 */
animal_is(albatross) :- albatross, !.
/* 未知 */
animal_is(unknown).             /* no diagnosis */

/* 知识库 */
/* 猪：哺乳动物、懒 */
pig :- mammal, 
      verify(lazy).
/* 企鹅：鸟类、不会飞、会游泳、黑白色 */ 
penguin :- bird, 
           verify(notFly), 
           verify(swims),
           verify(blackAndWhite).
/* 金钱豹：是哺乳动物、食肉、黄褐色、黑色斑点 */         
leopard :- mammal, 
           predator, 
           verify(tawnyColor),
           verify(darkSpots).
/* 长颈鹿：蹄类动物、有长脖子、长腿、暗斑点 */
giraffe :- ungulate, 
           verify(longNeck), 
           verify(longLegs).

/* 鸵鸟：鸟类、不会飞、长脖子、长腿、黑白色  */
ostrich :- bird,  
           verify(notFly), 
           verify(longNeck).
/* 斑马：黑色条纹、蹄类动物 */          
zebra :- ungulate,  
         verify(blackStripes).
/* 老虎：是哺乳动物、食肉、黄褐色、黑色条纹 */         
tiger :- mammal,  
         predator,
         verify(haveTawnyColor), 
         verify(haveBlackStripes).
/* 信天翁：鸟类、善于飞行 */         
albatross :- bird,
             verify(goodFlyer).

/* 分类标准 */
/* 哺乳动物 */
mammal    :- verify(haveHair), !.
mammal    :- verify(haveMilk).
 /* 鸟类 */
bird      :- verify(haveFeathers), !.
bird      :- verify(fly), 
             verify(layEgg).
/* 食肉动物 */ 
/* 吃肉 */              
predator :- verify(eatmeat), !.
/* 有犬齿、有爪子、眼睛在前方 */
predator :- verify(havePointedTeeth), 
             verify(haveClaws),
             verify(haveForwardEyes).
/* 蹄类动物 */             
ungulate :- mammal, 
            verify(haveHooves), !.
/* 反刍 */                
ungulate :- mammal, 
            verify(regurgitate).

/* 询问用户 */
ask(Question) :-
    write('Does the animal can: '),
    write(Question),
    write('? '),
    read(Response),
    nl,
    ( (Response == yes ; Response == y)
      ->
       assert(yes(Question)) ;
       assert(no(Question)), fail).

:- dynamic yes/1,no/1.

/* 询问事实，若不存在这个条件，则询问用户 */
verify(S) :-
   (yes(S) 
    ->
    true ;
    (no(S)
     ->
     fail ;
     ask(S))).

/* 将中间结果删除，以免影响下一次的判断 */
undo :- retract(yes(_)),fail. 
undo :- retract(no(_)),fail.
undo.