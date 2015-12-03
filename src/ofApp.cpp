#include "ofApp.h"
#include <iostream>
#include "stdio.h"
#include "string.h"
using namespace std;

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0, 0, 0);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofEnableAlphaBlending();
    ofSetBackgroundAuto(false);
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
    
    //ofPostProcessing
    post.init(ofGetWidth(), ofGetHeight());
    post.createPass<FxaaPass>();
        post.createPass<BloomPass>();
    isBloom =false;
    
    
    ofSetDataPathRoot("../Resources/data/");
    
    logo.loadImage("SDlogo.png");
    
    //文字1
    architxt.loadFont("SlimSansSerif-Bold.ttf",120);
    sprintf(text, "");
    float textW = architxt.stringWidth(text); //テキストの幅を調べる
    float textH = architxt.stringHeight(text);//テキストの高さを調べる
    fontX = ofGetWidth()/2-textW/2;
    fontY = ofGetHeight()/2-textH/2;//下に配置
    textNum = 0;
    
    //文字2
    architxt2.loadFont("foughtknight.ttf",50);
//    architxt2.loadFont("SlimSansSerif-Bold.ttf",45);
    sprintf(text2, "");
    textNum2 = 0;
    
    //文字3
    sprintf(text3, "");
    textNum3 = 0;
    
    //場面管理
    status = 0;
    
    //gui
    showGUI = false;
    gui.setup();
    gui.add(speed.setup("Speed",1,0,2));
    gui.add(vol.setup("Volume",0.6,0,1));
    //    gui.add(Saturation.setup("Saturation",255,0,255));
    //    gui.add(Brightness.setup("Brightness",255,0,255));
    
    //円の解像度
    ofSetCircleResolution(80);
    //Volume
    vol = 0.6f;
    
    //fft
    fft.setup();
    fft.setNumFFTBins(256);
    nBand= 256; //FFTの解像度の設定
    
    //円の回転
    rotate=180;
    
    //値を足す
    isAdding = false;
    
    
    //3D
    ofSetCircleResolution(24);
    ofSetSphereResolution(8);
    
    //3dparticle
    mesh.setMode(OF_PRIMITIVE_POINTS);
    pressed = false;
    for (int i = 0; i < num; i++) {
        particles[i].position = ofVec3f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()), ofRandom(-ofGetWidth(), ofGetWidth()));
        particles[i].depth = ofGetWidth();
    }
    
    //Initialize points offsets by random numbers
    for ( int j=0; j<300; j++ ) {
        tx[j] = ofRandom( 0, 1000 );
        ty[j] = ofRandom( 0, 1000 );
    }

    
}

void ofApp::reset(){

}


void ofApp::exit(){
    
}

//--------------------------------------------------------------
void ofApp::update(){
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    

    
    //fft
    fft.update();
    spectrum = fft.getSpectrum();
    
    //円の回転
    if(rotate >360){
        rotate = 0;
    }
    float flag = ofRandom(0,2);
    if (flag>0.66) {
        rotate-=0;
    }else{
        rotate+=0;
    }
    
    if (status==5) {//拡散
        post.begin();
        mesh.clear();
//        pressed = false;
//        float volume = fft.getUnScaledLoudestValue();
        float volume = fft.getMidVal();
        if (volume>1000) {
            press_x = ofRandom(0,ofGetWidth());
            press_y = ofRandom(0,ofGetHeight());
            pressed = true;
        }
        for (int i = 0; i < num; i++) {
            if (pressed) {
                particles[i].addAttractionForce(ofGetWidth()/2, ofGetHeight()/2 , 0, ofGetWidth() * 1.5, 1.0);
            }
            particles[i].update();
            particles[i].throughOffWalls();
            mesh.addVertex(ofVec3f(particles[i].position.x, particles[i].position.y, particles[i].position.z));
        }
        post.end();
    }
    
    
    //cloud
    if (status == 3||status==6) {
        //Get current spectrum with N bands
        float *val = ofSoundGetSpectrum( spectrum.size() );
        //We should not release memory of val,
        //because it is managed by sound engine
        
        //Update our smoothed spectrum,
        //by slowly decreasing its values and getting maximum with val
        //So we will have slowly falling peaks in spectrum
        for ( int i=0; i<spectrum.size(); i++ ) {
            spectrum[i] *= 0.97;	//Slow decreasing
            spectrum[i] = max( spectrum[i], val[i] );
        }
        
        //Update particles using spectrum values
        
        //Computing dt as a time between the last
        //and the current calling of update()
        float time = ofGetElapsedTimef();
        float dt = time - time0;
        dt = ofClamp( dt, 0.0, 0.1 );
        time0 = time; //Store the current time
        
        //Update Rad and Vel from spectrum
        //Note, the parameters in ofMap's were tuned for best result
        //just for current music track
        Rad = ofMap( spectrum[ bandRad ], 1, 3, 400, 800, true );
        Vel = ofMap( spectrum[ bandVel ], 0, 0.1, 0.05, 0.5 );
        //Update particles positions
        for (int j=0; j<spectrum.size(); j++) {
            if (pressed){
                tx[j] += Vel * dt/100;	//move offset
                ty[j] += Vel * dt/100;	//move offset
            }else{
                tx[j] += Vel * dt/1000;	//move offset
                ty[j] += Vel * dt/1000;	//move offset
            }
            
            //Calculate Perlin's noise in [-1, 1] and
            //multiply on Rad
            p[j].x = ofSignedNoise( tx[j] ) * Rad;		
            p[j].y = ofSignedNoise( ty[j] ) * Rad;	
        }
    }
    
    
    
}


//--------------------------------------------------------------
void ofApp::draw(){
    
    
    
    if (status==0) {
        
        
        
        ofBackground(252, 105, 33);
        ofSetColor(0,0,0);

        ofPushMatrix();
        //        ofTranslate(0, -ofGetHeight()*0.95);
        
        float textW = architxt.stringWidth(text); //テキストの幅を調べる
        float textH = architxt.stringHeight(text);//テキストの高さを調べる
        fontX = ofGetWidth()/2-textW/2;
        fontY = ofGetHeight()/2*1.1;//下に配置
        architxt.drawString(text, fontX, fontY);
        ofPopMatrix();
        
    }
    
    if (status==1) {
        ofBackground(0, 0, 0);
        post.begin();
        ofPushMatrix();
        ofTranslate(0,ofGetHeight()*0.85 );
        ofRotateX(180);
        logo.draw(-20, 80);
        ofPopMatrix();
        post.end();
    }
    
    
    
    if (status==2||status==10) {
        post.begin();
        
        
        //座標回転
//        ofSetColor(0,0,0,30); //フェードする色と透明度を設定
//        ofRect(0, 0, ofGetWidth(), ofGetHeight());
        col.set(45, 255, 254);
        
        ofPushMatrix();
        ofTranslate(ofGetWidth()/2,ofGetHeight()/2);
        ofRotateZ(rotate);
        
        //円の描写
        float width2 = 360/spectrum.size();
        
        int random = ofRandom(0, 15);
        int random2 = ofRandom(0,13);
        ofSetCircleResolution(50);
        
        
        
        for (int i =0 ; i<spectrum.size(); i++) {
            
            ofPushMatrix();
            //線
            if (i%6 == 1) {
                col.set(45, 255, 254);
                ofSetColor(col);
                float bar_x = ofGetWidth()/2-ofMap(i, 0, 255, 0, ofGetWidth()/2-85);
                ofRect(bar_x, 0, ofGetWidth()/spectrum.size()*2, (-spectrum[i]*vol)+2);
                ofRect(-bar_x, 0, ofGetWidth()/spectrum.size()*2, (-spectrum[i]*vol)+2);
                col.set(45, 255, 254,100);
                ofSetColor(col);
                ofRect(bar_x, 0, ofGetWidth()/spectrum.size()*2, (spectrum[i]*vol)+2);
                ofRect(-bar_x, 0, ofGetWidth()/spectrum.size()*2, (spectrum[i]*vol)+2);
            }
            
            //円
            
            if (i%2==1) {
            ofSetColor(0, 0, 0);
            ofCircle(0, 0 ,-vol*150+random2*2+5);
            int degree = ofMap(i, 0, 255, 0, 360,true);
            ofRotateZ(width2*degree);
            col.set(24,202,230);
//            col.set(45, 255, 254);
//            ofSetColor(col);
//            col.setHsb(i*360/256,255,255);
            ofSetColor(col);
            float addValue = 1;
            if (isAdding == true ){
                 addValue = ofRandom(3, 5);
            }
            ofRect(0, -vol*150+random2*2, width2*4, (-spectrum[i]*vol+5)*addValue);
            }
            ofPopMatrix();
            
            
        }
        
        
        ofPopMatrix();//円回転
        
        post.end();
    }
    
    //線　TV
    if (status==8) {
        post.begin();
        ofSetLineWidth(4);
        ofNoFill();
        ofPushMatrix();
        ofTranslate(0, ofGetHeight()/2);
        col.set(24,202,230);
        //        col.setHsb(360, 100, 100);
        ofSetColor(col);
        ofBeginShape();
        
        for (int i =0 ; i<spectrum.size(); i+=3) {
            float addValue;
            addValue = ofRandom(3,5);
            float add;
            add = ofRandom(-10,10 );
            int lineWidth = ofMap(i, 0, 255, 0,ofGetWidth() );
            if (spectrum[i]>30) {
                add = ofRandom(-2,2 );
            }
            if (isAdding) {
                add = add*addValue;
            }
            ofVertex(lineWidth, (spectrum[i])*add);
        }
        
//        ofVertex(ofGetWidth(), spectrum[spectrum.size()]);
//        ofVertex(ofGetWidth(), ofGetHeight());
        ofEndShape();
        
        
        ofBeginShape();
        col.set(24,202,230,100);
        ofSetColor(col);
        for (int i =0 ; i<spectrum.size(); i+=3) {
            float addValue;
            addValue = ofRandom(3,5);
            float add;
            add = ofRandom(-10,10 );
            int lineWidth = ofMap(i, 0, 255, 0,ofGetWidth() );
            if (spectrum[i]>30) {
                add = ofRandom(-2,2 );
            }
            if (isAdding) {
                add = add*addValue;
            }
            ofVertex(lineWidth, ofRandom(-20,20));
        }
        
       
        ofEndShape();
        
        ofPopMatrix();
        
        ofFill();

        post.end();
        
    }
    
    //never over
    if (status == 7) {
        post.begin();
        ofBackground(0);
        ofSetColor(24,202,230);
        ofPushMatrix();
        //        ofTranslate(0, -ofGetHeight()*0.95);
        ofTranslate(0,ofGetHeight()*0.85 );
        ofRotateX(180);
        textNum2 = 0;
        for (int i =0; i<16 ;i++) {
            sprintf(text2, textHolder2[textNum2]);
            fontX2 = (ofGetWidth()-20)/2*(i%2);
            fontY2 = (ofGetHeight()-30)/8*(i/2);
             string s1=textHolder3[textNum3];
             string s2=textHolder2[i];
            ofSetColor(20, 20, 20);
            architxt2.drawString(text2, 30+fontX2, fontY2-10);
            ofSetColor(24,202,230);
            if (s1==s2) {
                architxt2.drawString(text2, 30+fontX2, fontY2-10);
            }
            
            textNum2++;
            
        }
        ofPopMatrix();
        post.end();
    }
    
    //色
    if (status ==4||status==9) {
//        post.begin();
        ofBackground(0, 0, 0);
         ofPushMatrix();
        ofSetLineWidth(ofGetWidth() / float(spectrum.size()) / 2.0);
        ofSetCircleResolution(64);
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        
        for (int i = 0; i < spectrum.size(); i+=3) {
            
            // 色を設定
            float hue = ofMap(i, 0, spectrum.size(), 0, 160);
            float br = ofMap(spectrum[i], 0, 10, 0, 255,true);
            float radius = ofMap(spectrum[i],0, 600, 0, ofGetHeight() / 2.0,true);
           
            ofColor col;
            
            col.setHsb(160-hue, 255, br,127);
            
            ofSetColor(col);
            
            // 右
            
            float rx = ofMap(i, 0, spectrum.size(), 0, ofGetWidth() / 2.0);
            ofCircle(rx, 0, radius);
            
            // 左
            
            float lx = ofMap(i, 0, spectrum.size(), 0, -ofGetWidth() / 2.0); 
            
            ofCircle(lx, 0, radius);
            
        } 
        
        ofPopMatrix();
//        post.end();
        
    }
    
    if (status==5) {
        
        ofBackground(0,0,0);
        ofSetColor(45, 255, 254);
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        
        static GLfloat distance[] = { 0.0, 0.0, 1.0 };
        glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, distance);
        glPointSize(4000);
        mesh.draw();
        ofDisableBlendMode();
        
    }
    //cloud
    if (status==3||status==6) {
        ofBackground(0);
        
        //Draw cloud
        post.begin();
        
        //Move center of coordinate system to the screen center
        ofPushMatrix();
        ofTranslate( ofGetWidth() / 2, ofGetHeight() / 2 );
        
        //Draw points
        ofSetColor( 0, 0, 0 );
        ofSetColor(24,202,230);
        ofFill();
        for (int i=0; i<spectrum.size(); i++) {
            ofCircle( p[i], 2 );
        }
        
        //Draw lines between near points
        float dist = 40;	//Threshold parameter of distance
        for (int j=0; j<spectrum.size(); j++) {
            for (int k=j+1; k<spectrum.size(); k++) {
                if ( ofDist( p[j].x, p[j].y, p[k].x, p[k].y )
                    < dist ) {
                    ofLine( p[j], p[k] );
                }
            }
        }
        
        //Restore coordinate system
        ofPopMatrix();
         post.end();
        
    }
    
    
    
    ofSetColor(255);
    ;
    //    ofDrawBitmapString("v:vol- , V:vol+", 10, 115);
    //    ofDrawBitmapString("any:strings", 10, 130);
    //    ofDrawBitmapString("return:Visualizer", 10, 145);
    //    ofDrawBitmapString("left-arrow:back", 10, 160);
    
    
    //    movie.draw(20,20);
    
    //end
    if (status==11) {
        ofBackground(0, 0, 0);
    }
    
    if (showGUI) {
        gui.draw();
    }
    ofSetHexColor(0xffffff);
    string msg = "FPS:" + ofToString(ofGetFrameRate());//fps
//    ofDrawBitmapString(msg, 10, 100);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case '1':
            status = 1;
            break;
        case '2':
            status = 2;
            break;
        case '3':
            status = 3;
            break;
        case '4':
            status = 4;
            break;
        case '5':
            status = 5;
            break;
        case '6':
            status = 6;
            break;
        case '7':
            status = 7;
            break;
        case '8':
            status = 8;
            break;
        case '9':
            status = 9;
            break;
        case '0':
            status = 0;
            break;
            
        default:
            break;
    }
    
    if (key == 'g') {
//        showGUI = !showGUI;
    }
    if (key=='v') {
        vol = vol -0.1;
    }else if(key=='V'){
        vol = vol+0.1;
    }
    if (vol<0) vol = 0;
    if (vol>1) vol = 1;
    
    if (key==OF_KEY_RETURN||key==OF_KEY_RIGHT) {
        status++;
    }else if(key==OF_KEY_LEFT) {
        status--;
        post.init(ofGetWidth(), ofGetHeight());
    }else if(key==' '&&status==0){
        sprintf(text, textHolder[textNum]);
        textNum++;
        if (textNum>=62) {
            textNum=0;
        }
    }else if(key==' '&&(status==2||status==10)){//◯
        isAdding = true;
    }else if(key==' '&&status==8){//線
        isAdding = true;
    }else if(key==' '&&status==7){//never over
        sprintf(text3, textHolder3[textNum3]);
        textNum3++;
        if (textNum3>=16) {
            textNum3=0;
        }
    }else if(key==' '&&status==5){//拡散
        pressed = false;
        press_x=ofGetWidth()/2;
        press_y=ofGetHeight()/2;
    }else if(key==' '&&(status==3||status==6)){
        pressed = true;
    }
    
    
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key==' '&&(status==2||status==10)){//◯
        isAdding = false;
    }
    if(key==' '&&status==8){//線
        isAdding = false;
    }else if(key == ' '&&status ==5){//拡散
        pressed = true;
    }
    else if(key == ' '&&(status ==3||status==6)){//cloud
        pressed = false;
    }
    
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
     pressed = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
     pressed = false;
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
