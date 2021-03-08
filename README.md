# Leader-Election
Leader election algorithm for modular robots

authors:
  * [Bogdan Gorelkin](https://b.gorelkin.me)  <bogdan@gorelkin.me>
  * [Sheikh Shah Mohammad Motiur Rahman](https://motiur.info) <motiur@ieee.org>

supervisor:
  * [Abdallah Makhoul](https://www.femto-st.fr/en/femto-people/amakhoul) <abdallah.makhoul@univ-fcomte.fr>

project relaized in [VisibleSim](https://github.com/VisibleSim/VisibleSim)

---

## Introduction
<p>The algorithm is based in six phases. First, each module generates locally an integer, called weight based on the numbering of their connected interfaces. After that, spanning trees will be created using a recruitment method, then the values of the recruited modules will be calculated, the generated spanning tree will compete and the losing trees are dismantled, this step is repeated until one tree remains. And finally the leader is elected. 
</br><p>Computing the binary value and the weight of each block: For getting the number of interfaces we use getNbInterfaces() and generate a string by adding 1 and 0 based on the condition if connected or not respectively. After calculating the binary value, we converted the values to decimal to get the final weight value of each block. The output of this part is depicted in *Figure 1-a*.

<div align="center">
<img src="https://user-images.githubusercontent.com/74824667/110364380-f9614e00-8043-11eb-976c-0e6ed2eee4bf.png"></br>
<i>Figure 1-a: Calculation of binary number and weight value for each module</i>
</div>

The weight of each block is determined by the surfaces that have connections. Since the work was done in BlinkyBlocks, the weight of each block can take values from 0 to 32. Therefore, the potential leader can be the block whose weight is strictly equal to 1,2,4,8,16 or 32. That is, the block has only one connected neighbor (leaf).
</br>However, there are forms where each block has more than one connection. In this case, the possible leader will be selected by the lowest number of connected interfaces. That is, the block has connections to the sides that correspond to a smaller digit in a binary number:

![image](https://user-images.githubusercontent.com/74824667/110366310-65dd4c80-8046-11eb-976d-86592caf36c3.png)
*Figure1-b: Binary number of block’s interfaces*

Selected potential leaders begin to recruit their children. Leaders (in our shape it is blocks with B.ID: 5 and B.ID:7) send a message to child blocks and check if this block is occupied by other leaders. If the block is busy, it stops responding. Block with ID = 10 also trying to recruit block with ID = 9, but B.ID = 9 doesn’t respond because he is already busy by B.ID = 6 and B.ID = 7 as well. Recruitment continues until the last block is occupied. 

![image](https://user-images.githubusercontent.com/74824667/110366414-8d341980-8046-11eb-918f-9b6c4c1667ab.png)
*Figure 2: Recruitment blocks to get subtrees*

Finally, as many supposed leaders we have, as many subtrees we can get. To finally choose the leader, we must separately work with each subtree. Since there are two proposed leaders in our form, we got two trees, which are clearly displayed in *Figure 3*.

![image](https://user-images.githubusercontent.com/74824667/110366474-9f15bc80-8046-11eb-85d5-97ac8a478335.png)
*Figure 3: The resulting subtrees for further comparison*

After the subtrees are determined, the competition of trees begins, this can be called evolution, where the strongest with the highest weight wins and becomes the root of the tree. The last leaf of the subtree reports its weight to the parent. The parent sums the received weight with its own and passes the information on. This continues until the seed (suggested leader) won't get the total sum of its entire tree. *Figures 4-a and 4-b* show a detailed scheme for obtaining the weight of subtrees for the leaders B.ID = 5 and B.ID = 7, respectively.

![image](https://user-images.githubusercontent.com/74824667/110366717-f156dd80-8046-11eb-8233-05f08f185f2e.png)
*Figure 4-a: Getting the sum of the subtree for the leader B.ID = 5*

![image](https://user-images.githubusercontent.com/74824667/110366750-fae04580-8046-11eb-88a9-c8dbde5fe142.png)
*Figure 4-b: Getting the sum of the subtree for the leader B.ID = 7*

When the leaders know the sum of their trees, it remains only to compare the values of all the leaders and choose the winner. At this stage, we had great difficulties that slowed down our work. The problem is that we weren't focused on the status of each block. We tried to create an array in which the values of each tree would be written, but the problem is that two arrays of the same name were created and comparison was impossible.



 <sup id="a1">[1](#f1)</sup>
1.   <b id="f1">Link to the source</b> [↩](#a1)


