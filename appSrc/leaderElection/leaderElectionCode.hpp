#ifndef leaderElectionCode_H_
#define leaderElectionCode_H_

#include "robots/blinkyBlocks/blinkyBlocksSimulator.h"
#include "robots/blinkyBlocks/blinkyBlocksWorld.h"
#include "robots/blinkyBlocks/blinkyBlocksBlockCode.h"
/*
static const int BROADCAST_MSG_ID = 1001;
static const int ACKNOWLEDGE_MSG_ID = 1002;
static const int ANOTHERBROADCAST_MSG_ID = 1003;*/
static const int BROADCAST_MSG_ID = 1001;
static const int ACKNOWLEDGE_MSG_ID = 1002;
static const int RECRUIT_MSG_ID = 1005;
static const int SUM_MSG_ID = 1003;
static const int BROADCAST2_MSG_ID = 1004;
static const int TOTAL_MSG_ID = 1006;
static int NUM = 0;

using namespace BlinkyBlocks;

class LeaderElectionCode : public BlinkyBlocksBlockCode {
private:
	BlinkyBlocksBlock *module = nullptr;
	string binId;
	uint weight;
	bool isProspectiveLeader=false;
	//int interfaceStatus = 0;
	int distance=0;
	int currentRound=0;
	int nbWaitedAnswers=0;
//	bool recruited = false;
//	std::vector <int> interfaceStatus;
	std::vector <int> supposedLeader;
	std::vector <int> weightValue;
  std::vector <int> TotalChooseSum;
  std::vector <int> TreeMax;
	P2PNetworkInterface *parent= nullptr;
	int sumTotal = 0;
  int x = 0;
	int sended=0;
  int counter=0;
  int interfaceStatus[6];
   int totalConnectedInt=0;
   int bchoice=0;
  bool recruited=false;
  bool isLeaf=false;
	bool isLeader=false;
  int minNeighbors=999;
  bool diff_one=true;
  int total =0;
  int myLeader=0;
  int nbr=0;
public :
	LeaderElectionCode(BlinkyBlocksBlock *host);
	~LeaderElectionCode() {};

/**
  * This function is called on startup of the blockCode, it can be used to perform initial
  *  configuration of the host or this instance of the program.
  * @note this can be thought of as the main function of the module
  */
    void startup() override;

/**
  * @brief Message handler for the message 'broadcast'
  * @param _msg Pointer to the message received by the module, requires casting
  * @param sender Connector of the module that has received the message and that is connected to the sender
  */
  // void myBroadcastFunc(std::shared_ptr<Message>_msg,P2PNetworkInterface *sender);
//	 void myAnotherBroadcastFunc(std::shared_ptr<Message>_msg,P2PNetworkInterface *sender);
		void myBroadcastFunc(std::shared_ptr<Message>_msg,P2PNetworkInterface *sender);
		void myRecruitFunc(std::shared_ptr<Message>_msg,P2PNetworkInterface *sender);
		void myBroadcast2Func(std::shared_ptr<Message>_msg,P2PNetworkInterface *sender);

		void mySumFunc(std::shared_ptr<Message>_msg,P2PNetworkInterface *sender);
		void myTotalFunc(std::shared_ptr<Message>_msg,P2PNetworkInterface *sender);
/**
  * @brief Message handler for the message 'acknowledge'
  * @param _msg Pointer to the message received by the module, requires casting
  * @param sender Connector of the module that has received the message and that is connected to the sender
  */
   void myAcknowledgeFunc(std::shared_ptr<Message>_msg,P2PNetworkInterface *sender);
	// void myAcknowledgeFunc2(std::shared_ptr<Message>_msg,P2PNetworkInterface *sender);

/**
  * @brief Provides the user with a pointer to the configuration file parser, which can be used to read additional user information from each block config. Has to be overridden in the child class.
  * @param config : pointer to the TiXmlElement representing the block configuration file, all information related to concerned block have already been parsed
  *
  */
    void parseUserBlockElements(TiXmlElement *config) override;

/**
  * User-implemented keyboard handler function that gets called when
  *  a key press event could not be caught by openglViewer
  * @param c key that was pressed (see openglViewer.cpp)
  * @param x location of the pointer on the x axis
  * @param y location of the pointer on the y axis
  * @note call is made from GlutContext::keyboardFunc (openglViewer.h)
  */
    void onUserKeyPressed(unsigned char c, int x, int y) override;

/**
  * @brief This function is called when a module is tapped by the user. Prints a message to the console by default.
     Can be overloaded in the user blockCode
  * @param face face that has been tapped
  */
    void onTap(int face) override;

/**
  * Called by openglviewer during interface drawing phase, can be used by a user
  *  to draw a custom Gl string onto the bottom-left corner of the GUI
  * @note call is made from OpenGlViewer::drawFunc
  * @return a string (can be multi-line with `
`) to display on the GUI
  */
    string onInterfaceDraw() override;

/*****************************************************************************/
/** needed to associate code to module                                      **/
	static BlockCode *buildNewBlockCode(BuildingBlock *host) {
	    return(new LeaderElectionCode((BlinkyBlocksBlock*)host));
	}
/*****************************************************************************/
};

#endif /* leaderElectionCode_H_ */
