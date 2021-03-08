#include "leaderElectionCode.hpp"

LeaderElectionCode::LeaderElectionCode(BlinkyBlocksBlock *host):BlinkyBlocksBlockCode(host),module(host) {
    // @warning Do not remove block below, as a blockcode with a NULL host might be created
    //  for command line parsing
    if (not host) return;

    // Registers a callback (myBroadcastFunc) to the message of type R
    addMessageEventFunc2(BROADCAST_MSG_ID,
                       std::bind(&LeaderElectionCode::myBroadcastFunc,this,
                       std::placeholders::_1, std::placeholders::_2));

    // Registers a callback (myAcknowledgeFunc) to the message of type K
/*    addMessageEventFunc2(ACKNOWLEDGE_MSG_ID,
                       std::bind(&LeaderElectionCode::myAcknowledgeFunc,this,
                       std::placeholders::_1, std::placeholders::_2));

                       // Registers a callback (myAnotherBroadcastFunc) to the message of type R
  addMessageEventFunc2(ANOTHERBROADCAST_MSG_ID,
                                          std::bind(&LeaderElectionCode::myAnotherBroadcastFunc,this,
                                          std::placeholders::_1, std::placeholders::_2));
*/

// Registers a callback (myAcknowledgeFunc) to the message of type K
addMessageEventFunc2(ACKNOWLEDGE_MSG_ID,
                   std::bind(&LeaderElectionCode::myAcknowledgeFunc,this,
                   std::placeholders::_1, std::placeholders::_2));

addMessageEventFunc2(BROADCAST2_MSG_ID,
                   std::bind(&LeaderElectionCode::myBroadcast2Func,this,
                   std::placeholders::_1, std::placeholders::_2));

addMessageEventFunc2(RECRUIT_MSG_ID,
                   std::bind(&LeaderElectionCode::myRecruitFunc,this,
                   std::placeholders::_1, std::placeholders::_2));

addMessageEventFunc2(SUM_MSG_ID,
                   std::bind(&LeaderElectionCode::mySumFunc,this,
                   std::placeholders::_1, std::placeholders::_2));
addMessageEventFunc2(TOTAL_MSG_ID,
                   std::bind(&LeaderElectionCode::myTotalFunc,this,
                   std::placeholders::_1, std::placeholders::_2));

}

void LeaderElectionCode::startup() {
    console << "start " << module->blockId << "\n";

    binId = "";
    weight = 0;
    minNeighbors=module->getNbNeighbors();
     /*for(int i=0; i<module->getNbInterfaces(); i++) {
        if(module->getInterface(i)->isConnected()){
           interfaceStatus[i]=1;
           totalConnectedInt++;
           console << "bchoice = " << bchoice<< "\n";
           bchoice=bchoice + pow(2,i);

        }else{
            interfaceStatus[i]=0;
        }
    }
    console<<"weight = "<<bchoice<<"\n";
    total=bchoice;

*/


// Algorithm 1 Choice generation and interface set up
//  follow function needed for get total number of blocks from config file
    for(int i=0; i<module->getNbInterfaces(); i++) {
      if(module->getInterface(i)->isConnected()){
          binId += "1";
        //  interfaceStatus.push_back(1);
            totalConnectedInt++;
            interfaceStatus[i]=1;
            
    }else{
          binId += "0";
      //    interfaceStatus.push_back(0);
          interfaceStatus[i]=0;
    }

  }


  /*for (int i = 0; i < interfaceStatus.size(); i++)
  {
    console << "Interface Status: " << interfaceStatus[i] <<"\n";
  }*/

  int count = 0;

    for (int i = 0; i < binId.size(); i++)
    {
        if (binId[i] ==  '1')
        {
            ++ count;
        }

    }


    int weight = std::strtol(binId.c_str(), nullptr, 2);
    bchoice = weight;
     console<<"weight = "<<bchoice<<"\n";
    total=bchoice; // weight value of each block to calculate in future weight of subTree
    
    
    //onsole << BlinkyBlocksWorld::getWorld()->maxBlockId <<"\n";
    //weightValue.push_back(weight);
  //  console <<"Weight Value: " << weight <<"\n";
    /*for (size_t i = 0; i < weightValue.size(); i++) {
    console << weightValue[i];
  }*/


    if (count == 1){//count equal how many neighbors have your block
      supposedLeader.push_back(module->blockId);
    }

    for (int i = 0; i < supposedLeader.size(); i++) {

      if (module->blockId == supposedLeader[i]) {
      //  console << "weight [i]" << weightValue[i] <<"\n";
      //  console << "block Id: " << module->blockId <<"\n";
      //  console << "i" << i  <<"\n";
        module->setColor(RED);
        isProspectiveLeader = true;
      }
    }

    if (isProspectiveLeader) {
      //  console << weight <<"\n";
        //console << weight <<"\n";
        module->setColor(RED);
        currentRound=1;
        distance=0;
        nbWaitedAnswers=sendMessageToAllNeighbors("distance(1,1)",new MessageOf<pair<int,int>>(BROADCAST_MSG_ID,make_pair(weight,module->blockId)),1000,100,0);
        console <<"NBWaited: " << nbWaitedAnswers <<"\n";
	} else {
        currentRound=0;
    }


}


void LeaderElectionCode::myBroadcastFunc(std::shared_ptr<Message>_msg, P2PNetworkInterface*sender) {
    MessageOf<pair<int,int>>* msg = static_cast<MessageOf<pair<int,int>>*>(_msg.get());
    pair<int,int> msgData = *msg->getData();
    int msgDistance = msgData.first;
    //console << "rec. distance(" << msgData.first << "," << msgData.second << ") from " << sender->getConnectedBlockId() << "\n";
    if (parent==nullptr || distance>msgDistance) {
        parent = sender;
        distance = msgDistance;
       // setColor(distance);
        sendMessageToAllNeighbors(" ",new MessageOf<pair<int,int>>(BROADCAST_MSG_ID,make_pair(distance+1,currentRound)),1000,100,1,sender);
    }



    if (parent==nullptr || msgData.second>currentRound || (msgData.second==currentRound && msgData.first<distance)) {
        distance=msgData.first;
        //setColor(distance);
        currentRound=msgData.second;
        parent=sender;
        isLeader=false;
        string str="distance(";
        str+=to_string(distance+1)+","+to_string(currentRound)+")";
        nbWaitedAnswers=sendMessageToAllNeighbors(str.c_str(),new MessageOf<pair<int,int>>(BROADCAST_MSG_ID,make_pair(distance+1,currentRound)),1000,100,1,sender);
        if (nbWaitedAnswers==0) {
            sendMessage(" ",new MessageOf<int>(ACKNOWLEDGE_MSG_ID,minNeighbors),parent,1000,100);
            //sendMessage("neighnors number",new MessageOf<int>(NUM_MSG_ID,module->getNbNeighbors()),parent,1000,100);

        }
    } else {
        sendMessage(" ",new MessageOf<int>(ACKNOWLEDGE_MSG_ID,minNeighbors),sender,1000,100);
    }
}

void LeaderElectionCode::myAcknowledgeFunc(std::shared_ptr<Message>_msg, P2PNetworkInterface*sender) {
    MessageOf<int>* msg = static_cast<MessageOf<int>*>(_msg.get());
    int msgData = *msg->getData();
    console<<msgData<<"\n";
    nbWaitedAnswers--;
    if(msgData<minNeighbors){
            minNeighbors=msgData;
            //console<<"new min neighbors= "<<minNeighbors<<"\n";
    }
    console << "rec. Ack(" << nbWaitedAnswers << ") from " << sender->getConnectedBlockId() << "\n";
    if (nbWaitedAnswers==0) {
        if (parent==nullptr) {
            setColor(WHITE);
            sendMessageToAllNeighbors("min neighbor broadc2",new MessageOf<int>(BROADCAST2_MSG_ID,minNeighbors),1000,100,0);
        } else {
            sendMessage("ack2parent",new MessageOf<int>(ACKNOWLEDGE_MSG_ID,minNeighbors),parent,1000,100);
            //sendMessage("neighnors number",new MessageOf<int>(NUM_MSG_ID,module->getNbNeighbors()),parent,1000,10);
        }
    }
}


void LeaderElectionCode::myBroadcast2Func(std::shared_ptr<Message>_msg, P2PNetworkInterface*sender) {
    MessageOf<int>* msg = static_cast<MessageOf<int>*>(_msg.get());
    int msgData = *msg->getData();
    if(msgData==module->getNbNeighbors()){
        NUM++;
        setColor(NUM);
        isLeader=true;
        parent=nullptr;
        //recruit msg sending
        nbr=sendMessageToAllNeighbors("Recruit msg",new MessageOf<pair<int,int>>(RECRUIT_MSG_ID,make_pair(module->blockId,NUM)),1000,100,0);
    }
    if(sended==0){
    sendMessageToAllNeighbors("min neighbor broadc2",new MessageOf<int>(BROADCAST2_MSG_ID,msgData),1000,100,0,parent);
    sended=1;
    }
}

// Algorithm 2: Recived recruit message
void LeaderElectionCode::myRecruitFunc(std::shared_ptr<Message>_msg, P2PNetworkInterface*sender) {
    console<<"recruitment received"<<"\n";
    MessageOf<pair<int,int>>* msg = static_cast<MessageOf<pair<int,int>>*>(_msg.get());
    pair<int,int> msgData = *msg->getData();

/*    if(recruited == false){
        console<<"not yet recruited"<<"\n";
    }
    if(recruited == true){
        console<<"RECRUITED!!"<<"\n";
    }

    if(isLeader == true){
        console<<"SRY SIR I'M A LEADER"<<"\n";
    }  */
    if(recruited==false && isLeader==false){
        myLeader=msgData.first;
        setColor(msgData.second);
        console<<"\n"<<"my leader is ::: "<<myLeader<<"\n";
        recruited=true;
        //parent=nullptr;
        parent=sender;
        int b = module->getInterfaceBId(sender);
        interfaceStatus[b]=3;
      /*  for(int i=0; i<module->getNbInterfaces(); i++){
            if(interfaceStatus[i]==1){
                diff_one=false;
                break;
            }
        }
        if(diff_one==true){
            if (isLeaf==true)
            {
                total=bchoice;
                //send sum msg to parent
            }
        */
       // }else{
            sendMessageToAllNeighbors("Recruit msg",new MessageOf<pair<int,int>>(RECRUIT_MSG_ID,make_pair(msgData.first,msgData.second)),1000,100,0 /*,parent*/);
            if(parent!=nullptr){
            sendMessage("SUM Msg", new MessageOf<int>(SUM_MSG_ID,bchoice),
                    parent, 100, 200);
            }
       // }
    }

}



void LeaderElectionCode::mySumFunc(std::shared_ptr<Message>_msg, P2PNetworkInterface*sender) {
    MessageOf<int>* msg = static_cast<MessageOf<int>*>(_msg.get());
    int msgData = *msg->getData();
    if(parent==nullptr && isLeader){
        //output the total...
        total=total+msgData;
        console<<"\n"<<" msgData = "<<msgData<<"\n";
        counter++;
        console<<"\n"<<" New sum = "<<total<<"!"<<"\n";
       // TotalChooseSum.push_back(total);
        console<<"\n"<<"New Recruits = "<<counter<<"!"<<"\n";
        nbr--;
        /*if(nbr==0){
            console << "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa   nbr = 0"<< "\n";
            sendMessageToAllNeighbors("SENDING TOTAL msg",new MessageOf<pair<int,int>>(TOTAL_MSG_ID,make_pair(total,module->blockId)),1000,100,0 /*,parent/);
        }*/
    }else{
        sendMessage("SUM Msg", new MessageOf<int>(SUM_MSG_ID,msgData),
                   parent, 100, 200);
        //sendMessageToAllNeighbors("SENDING TOTAL msg",new MessageOf<pair<int,int>>(TOTAL_MSG_ID,make_pair(total,module->blockId)),1000,100,0 /*,parent*/);
    }
    
    //console << "rec. distance(" << msgData << ",";
    /*    std::vector<int>::iterator result;
 
    result = std::max_element(TotalChooseSum.begin(), TotalChooseSum.end());

     for (int i = 0; i < TotalChooseSum.size(); i++)
    {
            

        console<<"\n"<<" i = "<<i<<"!";
        console<<"\n"<<" TotalChooseSum = "<<TotalChooseSum[i]<<"!"<<"\n";
        //weightValue.push_back() ;
        console<<"\n"<<" max = "<<result[0]<<"!";     
    }*/    
}


void LeaderElectionCode::myTotalFunc(std::shared_ptr<Message>_msg, P2PNetworkInterface*sender) {

    MessageOf<pair<int,int>>* msg = static_cast<MessageOf<pair<int,int>>*>(_msg.get());
    pair<int,int> msgData = *msg->getData();
    //console << "total: "<< msgData.first<<"\n";
   // console << "block ID: "<< msgData.second<<"\n";
    
    if (msgData.second == myLeader)
    {
        //total = msgData.first;
        //TotalChooseSum.push_back(total);
        //console<<"NEW TOTAL HAS BEEN CHANGED TO "<<total<<"\n";
        sumTotal = msgData.first;
        if(sumTotal>x){
            x = sumTotal;
            console<<"NEW TOTAL HAS BEEN CHANGED TO "<<x<<"\n";
        }

    }
    if(msgData.second!=myLeader){
         console<<"total is greater";
    }
   /* if (myLeader)
    {
        setColor(BLACK);
    }
    
    if(msgData.second!=myLeader){
        console<<"318 MY NEW LEADER IS = "<<myLeader;
        if(msgData.first>bchoice){
            parent=nullptr;
            parent=sender;
            myLeader=msgData.second;
            //console<<"MY NEW LEADER IS = "<<myLeader;
        }else
        {
            if(x == 0){
                console<<"MY NEW LEADER IS = "<<myLeader;
                sendMessageToAllNeighbors("SENDING TOTAL msg",new MessageOf<pair<int,int>>(TOTAL_MSG_ID,make_pair(msgData.first,msgData.second)),1000,100,0 , parent);
                x = 1;
            }
            
            
        }
        

    }*/
}

/*
void LeaderElectionCode::myBroadcastFunc(std::shared_ptr<Message>_msg, P2PNetworkInterface*sender) {

  MessageOf<pair<int,int>>* msg = static_cast<MessageOf<pair<int,int>>*>(_msg.get());
  pair<int,int> msgData = *msg->getData();
  int msgDistance = msgData.first;
//  console << "rec. distance(" << msgData.first << "," << msgData.second << ") from " << sender->getConnectedBlockId() << "\n";
  if (parent==nullptr || distance>msgDistance) {
//  if (distance>msgDistance) {
      parent = sender;
      distance = msgDistance;
      setColor(GREY);
      sendMessageToAllNeighbors("distance",new MessageOf<pair<int,int>>(BROADCAST_MSG_ID,make_pair(distance+1,currentRound)),1000,100,1,sender);
  }

}

void LeaderElectionCode::myAnotherBroadcastFunc(std::shared_ptr<Message>_msg, P2PNetworkInterface*sender) {

  MessageOf<pair<int,int>>* msg = static_cast<MessageOf<pair<int,int>>*>(_msg.get());
  pair<int,int> msgData = *msg->getData();

  int receivedWeight = msgData.first;
  //console <<"weight message: " << receivedWeight << "\n";
  //console << "rec message " << msgData.first << "," << msgData.second << ") from " << sender->getConnectedBlockId() << "\n";
  //console <<"binary id: "<< binId;

  //sumTotal = receivedWeight + weightValue[0];
  //console <<"My weight message: " << weightValue[0] << "\n";
  //console << "Total I have: " << (receivedWeight + weightValue[0]) <<"\n";
//  if (recruited && nbWaitedAnswers) {
  sumTotal = weightValue[0] + msgData.first;
//}
  if (!recruited && !parent) {

    recruited = true;
    parent = sender;
    interfaceStatus.push_back(3);
    currentRound=msgData.second;
    parent=sender;
    string str="weight(";
    str+=to_string(weightValue[0])+","+to_string(currentRound)+")";
    sendMessageToAllNeighbors(str.c_str(),new MessageOf<pair<int,int>>(ANOTHERBROADCAST_MSG_ID,make_pair(weightValue[0],module->blockId)),1000,100,1,sender);

}
if (nbWaitedAnswers==0) {
          sendMessage("ack2parent",new MessageOf<pair<int,int>>(ACKNOWLEDGE_MSG_ID, make_pair(sumTotal,module->blockId)),parent,1000,100);
      }

}


void LeaderElectionCode::myAcknowledgeFunc(std::shared_ptr<Message>_msg, P2PNetworkInterface*sender) {
  MessageOf<pair<int,int>>* msg = static_cast<MessageOf<pair<int,int>>*>(_msg.get());
  pair<int,int> msgData = *msg->getData();

  int receivedWeight = msgData.first;
//  console <<"Received weight message: " << receivedWeight << "\n";
  console << "My weight message: " << weightValue[0] << "\n";
//  console << "Total I have: " << (receivedWeight + weightValue[0]) <<"\n";
  nbWaitedAnswers--;
  //sumTotal = receivedWeight + weightValue[0];
  //console <<"My weight message: " << weightValue[0] << "\n";
//  console << "Total I have: " << (receivedWeight + weightValue[0]) <<"\n";
//  console << "rec. Ack(" << nbWaitedAnswers << ") from " << sender->getConnectedBlockId() << "\n";

//if (recruited && nbWaitedAnswers) {
      sumTotal = weightValue[0] + msgData.first;
      console <<"Total Sum: "<< sumTotal << "\n";
//}
  console << "rec. Weight(" << receivedWeight << ") from " << sender->getConnectedBlockId() << "\n";

  if (nbWaitedAnswers==0) {
      if (parent==nullptr) {
          setColor(WHITE);
      } else {
          sendMessage("ack2parent",new MessageOf<pair<int,int>>(ACKNOWLEDGE_MSG_ID, make_pair(sumTotal,module->blockId)),parent,1000,100);
      }
  }

}*/

void LeaderElectionCode::parseUserBlockElements(TiXmlElement *config) {
    const char *attr = config->Attribute("leader");
    if (attr!=nullptr) {
        std::cout << module->blockId << " is leader!" << std::endl; // complete with your code
    }
}

void LeaderElectionCode::onUserKeyPressed(unsigned char c, int x, int y) {
    switch (c) {
        case 'a' : // update with your code
        break;
    }
}
void LeaderElectionCode::onTap(int face) {
    std::cout << "Block 'tapped':" << module->blockId << std::endl; // complete with your code

}

string LeaderElectionCode::onInterfaceDraw() {
    return "My text\nIn two lines"; // to update with your text
}
