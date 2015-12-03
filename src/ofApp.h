#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxPostProcessing.h"
#include "ofxProcessFFT.h"
#include <vector>
#include "ParticleVec3.h"

using namespace std;

#define INPUT_WIDTH   640
#define INPUT_HEIGHT   480


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    bool showGUI;
    
    ofxPanel gui;
    ofxFloatSlider speed;
    ofxFloatSlider vol;
    ofxIntSlider Saturation;
    ofxIntSlider Brightness;
    
    ofSoundPlayer mySound;
    
    float pan;//パン
    
    //    float* fft;//FFTの結果を格納する配列
    int nBand; //FFTの解像度
    
    //fft
    ProcessFFT fft;
    vector<float> spectrum;
    
    ofColor col;
    
    int rotate;
    
    bool isAdding;
    
    //image
    ofImage logo;
    
    ofxPostProcessing post;
    
    int status;
    
    //text1
    ofTrueTypeFont architxt;
    char textHolder[255][16]={
        "BUY", "USE", "BREAK", "FIX",
        "TRASH", "CHANGE", "MAIL" ,"UPGRADE",
        "CHARGE", "POINT", "ZOOM", "PRESS",
        "SNAP", "WORK", "QUICK" , "ERASE",
        "WRITE", "CUT", "PASTE", "SAVE",
        "LOAD", "CHECK", "QUICK","RERWITE",
        "PLUG", "PLAY", "BURN", "RIP",
        "DRAG", "DROP", "ZIP" , "UNZIP",
        "LOCK", "FILL", "CALL", "FIND",
        "VIEW", "CODE", "JAM" , "UNLOCK",
        "SURF", "SCROLL", "PAUSE", "CLICK",
        "CROSS", "CRACK", "SWITCH" , "UPDATE",
        "NAME", "RATE", "TUNE", "PRINT",
        "SCAN", "SEND", "FAX" , "RENAME",
        "TOUCH", "BRING", "PAY", "WATCH",
        "TECHNOLOGIC"
    };
    
    /*"Buy it", "use it", "break it", "fix it",
     "Trash it", "change it", "mail" ,"upgrade it",
     "Charge it", "point it", "zoom it", "press it",
     "Snap it", "work it", "quick" , "erase it",
     "Write it", "cut it", "paste it", "save it",
     "Load it", "check it", "quick","rewrite it",
     "Plug it", "play it", "burn it", "rip it",
     "Drag and", "drop it", "zip" , "unzip it",
     "Lock it", "fill it", "call it", "find it",
     "View it", "code it", "jam" , "unlock it",
     "Surf it", "scroll it", "pause it", "click it",
     "Cross it", "crack it", "switch" , "update it",
     "Name it", "rate it", "tune it", "print it",
     "Scan it", "send it", "fax" , "rename it",
     "Touch it", "bring it", "pay it", "watch it",
     "Technologic"*/
    
    char text[255];
    int fontX; //フォントX軸の位置の宣言
    int fontY;
    int textNum;
    
    //text2
    ofTrueTypeFont architxt2;
    char textHolder2[32][16] = {
        "after","harder",
        "make it","ever",
        "Do it", "work is",
        "stronger","  over",
        "never","Work it",
        "better","faster",
        "More than","   Our",
        "makes us","  hour",
    };

    char text2[255];
    int fontX2;
    int fontY2;
    int textNum2;
    
    //text2判定用tex3
    char textHolder3[32][16] = {
        "Work it","harder", "make it","better",
        "Do it", "faster", "makes us","stronger",
        "More than","ever", "  hour", "after",
        "   Our","work is","never","  over"
    };
    char text3[255];
    int textNum3;
    
    bool isBloom;
    int bw;
    
    ofVideoPlayer movie;
    
    void reset();
    ofImage img;
    ofImageQualityType quality;
    
    //3D
    ofIcoSpherePrimitive icoSphere;
    ofConePrimitive cone;
    ofBoxPrimitive box;
    
    //3dparticle
    static const int num = 500;
    ParticleVec3 particles[num];
    ofVboMesh mesh;
    bool pressed;
    float press_x ;
    float press_y;
    
    //cloud
    float Rad = 500;		//Cloud raduis parameter
    float Vel = 0.1;		//Cloud points velocity parameter
    int bandRad = 5;		//Band index in spectrum, affecting Rad value
    int bandVel = 100;		//Band index in spectrum, affecting Vel value
    
    
    //Offsets for Perlin noise calculation for points
    float tx[300], ty[300];
    ofPoint p[300];			//Cloud's points positions
    
    float time0 = 0;		//Time value, used for dt computing
};
