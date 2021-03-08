# Leader-Election
Leader election algorithm for modular robots

authors:
  * [Bogdan Gorelkin](https://b.gorelkin.me)  <bogdan@gorelkin.me>
  * [Sheikh Shah Mohammad Motiur Rahman](https://motiur.info) <motiur@ieee.org>

supervisor:
  * [Abdallah Makhoul](https://www.femto-st.fr/en/femto-people/amakhoul) <abdallah.makhoul@univ-fcomte.fr>

project relaized in [VisibleSim](https://github.com/VisibleSim/VisibleSim)

---

##  Introduction
The algorithm is based in six phases. First, each module generates locally an integer, called weight based on the numbering of their connected interfaces. After that, spanning trees will be created using a recruitment method, then the values of the recruited modules will be calculated, the generated spanning tree will compete and the losing trees are dismantled, this step is repeated until one tree remains. And finally the leader is elected. 
</br>Computing the binary value and the weight of each block: For getting the number of interfaces we use getNbInterfaces() and generate a string by adding 1 and 0 based on the condition if connected or not respectively. After calculating the binary value, we converted the values to decimal to get the final weight value of each block. The output of this part is depicted in <i>Figure 1-a</i>.

<div align="center">
<img src="https://user-images.githubusercontent.com/74824667/110364380-f9614e00-8043-11eb-976c-0e6ed2eee4bf.png"></br>
<i>Figure 1-a: Calculation of binary number and weight value for each module</i>
</div>
</br>

The weight of each block is determined by the surfaces that have connections. Since the work was done in BlinkyBlocks, the weight of each block can take values from 0 to 32. Therefore, the potential leader can be the block whose weight is strictly equal to 1,2,4,8,16 or 32. That is, the block has only one connected neighbor (leaf).
</br>However, there are forms where each block has more than one connection. In this case, the possible leader will be selected by the lowest number of connected interfaces. That is, the block has connections to the sides that correspond to a smaller digit in a binary number:

<div align="center">
<img src="https://user-images.githubusercontent.com/74824667/110366310-65dd4c80-8046-11eb-976d-86592caf36c3.png"></br>
<i>Figure1-b: Binary number of block’s interfaces</i>
</div>
</br>

Selected potential leaders begin to recruit their children. Leaders (in our shape it is blocks with B.ID: 5 and B.ID:7) send a message to child blocks and check if this block is occupied by other leaders. If the block is busy, it stops responding. Block with ID = 10 also trying to recruit block with ID = 9, but B.ID = 9 doesn’t respond because he is already busy by B.ID = 6 and B.ID = 7 as well. Recruitment continues until the last block is occupied. 

<div align="center">
<img src="https://user-images.githubusercontent.com/74824667/110366414-8d341980-8046-11eb-918f-9b6c4c1667ab.png"></br>
<i>Figure 2: Recruitment blocks to get subtrees</i>
</div>
</br>

Finally, as many supposed leaders we have, as many subtrees we can get. To finally choose the leader, we must separately work with each subtree. Since there are two proposed leaders in our form, we got two trees, which are clearly displayed in <i>Figure 3</i>.

<div align="center">
<img src="https://user-images.githubusercontent.com/74824667/110366474-9f15bc80-8046-11eb-85d5-97ac8a478335.png"></br>
<i>Figure 3: The resulting subtrees for further comparison</i>
</div>
</br>

After the subtrees are determined, the competition of trees begins, this can be called evolution, where the strongest with the highest weight wins and becomes the root of the tree. The last leaf of the subtree reports its weight to the parent. The parent sums the received weight with its own and passes the information on. This continues until the seed (suggested leader) won't get the total sum of its entire tree. <i>Figures 4-a and 4-b</i> show a detailed scheme for obtaining the weight of subtrees for the leaders B.ID = 5 and B.ID = 7, respectively.

<div align="center">
<img src="https://user-images.githubusercontent.com/74824667/110366717-f156dd80-8046-11eb-8233-05f08f185f2e.png"></br>
<i>Figure 4-a: Getting the sum of the subtree for the leader B.ID = 5</i>
</div>
</br>

<div align="center">
<img src="https://user-images.githubusercontent.com/74824667/110366750-fae04580-8046-11eb-88a9-c8dbde5fe142.png"></br>
<i>Figure 4-b: Getting the sum of the subtree for the leader B.ID = 7</i>
</div>
</br>

When the leaders know the sum of their trees, it remains only to compare the values of all the leaders and choose the winner. At this stage, we had great difficulties that slowed down our work. The problem is that we weren't focused on the status of each block. We tried to create an array in which the values of each tree would be written, but the problem is that two arrays of the same name were created and comparison was impossible.

## 	Experimental part
In the next paragraph, we provide the results of our simulation, as well as the execution time of the algorithm and the number of messages sent.
<div align="center">
<img src="https://user-images.githubusercontent.com/74824667/110368761-c621bd80-8049-11eb-96b8-c0aeeda3604b.png"></br>
<i>Figure 5: Final weight of subtrees</i>
</div>
</br>
The process seems less complicated when everything is obvious in the configuration model. The process diagram is shown in <i>Figure 6-a</i> and experimented in <i>Figure 6-b</i>.
</br>
<div align="center">
<img src="https://user-images.githubusercontent.com/74824667/110368891-eb163080-8049-11eb-8ee7-90110caeebbf.png"></br>
<i>Figure 6-a: diagram for obvious model</i>
</div>
</br>

<div align="center">
<img src="https://user-images.githubusercontent.com/74824667/110368974-02551e00-804a-11eb-83a1-88db6fbbf80f.png"></br>
<i>Figure 6-b: simulation of obvious model</i>
</div>
</br>

In the course of the work, we encountered difficulties that we could not solve before meet the deadline. One of these difficulties arises when hiring blocks when there is a block in the middle between the two possible leaders. That is, this block has the same distance to the leaders. 
<div align="center">
<img src="https://user-images.githubusercontent.com/74824667/110368993-07b26880-804a-11eb-9559-9dce66712e94.png"></br>
<i>Figure 7a: model with an undefined block</i>
</div>
</br>
This problem is shown schematically in <i>Figure 7-a</i>. Block with ID number 4 simultaneously receives messages from possible leaders and tries to answer them. Because of this, the program runs in an infinite loop and the program execution time does not stop. This can be seen when simulating this configuration in <i>Figure 7-b</i>.</br>
<div align="center">
<img src="https://user-images.githubusercontent.com/74824667/110369181-519b4e80-804a-11eb-9734-2914c973badd.png"></br>
<i>Figure 7-b: Simulating a model with an undefined block</i>
</div>
</br>
A simulation of the program was also performed for 199 blocks, this is demonstrated schematically in <i>Figure 8-a</i> and experimentally in the <i>Figure 8-b</i>.</br>
<div align="center">
<img src="https://user-images.githubusercontent.com/74824667/110369198-58c25c80-804a-11eb-8b70-340ef017a5f3.png"></br>
<i>Figure 8-a: Configuration model diagram for 199 blocks</i>
</div>
</br>
<div align="center">
<img src="https://user-images.githubusercontent.com/74824667/110369219-5fe96a80-804a-11eb-8c88-c45489b9af3b.png"></br>
<i>Figure 8b: Experiment on 199 blocks</i>
</div>
</br>
When the configuration model contains blocks with only one neighbor (that is, the weight of which is exactly 1, 2, 4, 8, 16, 32), there are no problems. But when there is no such block, the choice of the leader becomes more difficult. Our code is static, to improve it we need to make the part shown in <i>Figure 9</i> dynamic.</br>
```cpp
if (count == 1){
//count equal to how many neighbors have your block
supposedLeader.push_back(module->blockId);
}
```
<div align="center">
<i>Figure 9: Weak point in our work</i>
</div>
</br>

An experiment for a volumetric cube is shown in <i>Figure 10</i>. This cube consists of small cubes (modular robots). For the program to start working, the "count" must be equal to 3, this is the number of neighbors for possible leaders.
<div align="center">
<img src="https://user-images.githubusercontent.com/74824667/110370889-8f00db80-804c-11eb-8dcc-41131a33f428.png"></br>
<i>Figure 10: Volumetric cube model</i>
</div>
</br>

However, when the program is executed, the same problem arises as in <i>Figure 7-a</i>, but on a large scale. To analyze this problem, let's turn to <i>Figure 11</i> in more detail.
<div align="center">
<img src="https://user-images.githubusercontent.com/74824667/110371006-af309a80-804c-11eb-96be-53b88384669a.png"></br>
<i>Figure 11: Analysis of the reasons for ambiguity</i>
</div>
</br>

According to the proposed algorithm, recruiting starts with the blocks that have the least number of neighbors (and also the least weight). That is, from the blocks that are in the corners of the rectangle. The first recruitment step is successful, then blocks that are colored red are trying to recruit gray blocks from the center. Recruiting fails, because the gray box does not know which request needs to be answered. This situation is considered separately in schematic <i>Figure 12</i>. Upon detailed examination, it becomes clear that this situation is no different from <i>Figure 7-a</i>.
<div align="center">
<img src="https://user-images.githubusercontent.com/74824667/110371154-dedfa280-804c-11eb-877d-dd0d9aa9a2b5.png"></br>
<i>Figure 12: Detailed examination of issue</i>
</div>
</br>
	Also, this experiment was carried out for a random model, the results of which are shown in <i>Figure 13</i>.</br>
<div align="center">
<img src="https://user-images.githubusercontent.com/74824667/110372359-7db8ce80-804e-11eb-86aa-dd104e686736.png"></br>
<i>Figure 13: Experimenting with a random model</i>
</div>
</br>

##  Conclusions
This algorithm is needed not only to play the evolution of digital trees, but also has a very useful and important application. in decentralized systems where the task is to select a leader, this algorithm can be used on an equal footing with the ABC-Center, k-BFS SumSweep, etc.
</br>This work is quite voluminous. The operation of the algorithm is clear and transparent, but when implementing the algorithm, a large number of difficulties arise that must be solved.




