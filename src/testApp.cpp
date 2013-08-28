#include "testApp.h"
#include "Utilities.h"
#include "Helpers.h"



//--------------------------------------------------------------
void testApp::setup(){
    cameraSize.width=640;
    cameraSize.height=480;
    
    ofSetVerticalSync(true);
	cam.initGrabber(cameraSize.width,cameraSize.height);
   
   

}

//--------------------------------------------------------------
void testApp::update(){
    
    cam.update();
	if(!cam.isFrameNew()) {
        return;
    }
    
//    const char* filename =  "babel5.jpg";
//    
//    //    Mat I = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
//    //    if( I.empty())
//    //        return -1;
//    
//    
//    image.loadImage(filename);
    
//    image=cam;
    
    if(image.getWidth()){
        image.allocate(cam.getWidth(), cam.getHeight(), OF_IMAGE_COLOR);
    }
    
    image.setFromPixels(cam.getPixelsRef());
    
    Mat  I = toCv(image);
    I.convertTo(I, CV_32F);
    cvtColor(I, I, CV_BGR2GRAY);
    
    Mat padded;                            //expand input image to optimal size
    int m = getOptimalDFTSize( I.rows );
    int n = getOptimalDFTSize( I.cols ); // on the border add zero values
    copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));
    
    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexI;
    merge(planes, 2, complexI);         // Add to the expanded another plane with zeros
    
    dft(complexI, complexI);            // this way the result may fit in the source matrix
    
    // compute the magnitude and switch to logarithmic scale
    // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
    split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
    magI = planes[0];
    
    magI += Scalar::all(1);                    // switch to logarithmic scale
    log(magI, magI);
    
    // crop the spectrum, if it has an odd number of rows or columns
    magI = magI(cv::Rect(0, 0, magI.cols & -2, magI.rows & -2));
    
    // rearrange the quadrants of Fourier image  so that the origin is at the image center
    int cx = magI.cols/2;
    int cy = magI.rows/2;
    
    Mat q0(magI, cv::Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(magI, cv::Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(magI, cv::Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(magI, cv::Rect(cx, cy, cx, cy)); // Bottom-Right
    
    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    
    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);
    
    normalize(magI, magI, 0, 1, CV_MINMAX); // Transform the matrix with float values into a
    // viewable image form (float between values 0 and 1).
    
    //    freq= ofImage();
    freq.allocate(magI.rows, magI.cols  , OF_IMAGE_GRAYSCALE  );
    //    magI.convertTo(magI, CV_8U  );
    printf("magI channels %d\n",magI.channels());
    
    //    toOf(I, image.getPixelsRef());
    //        toOf(magI, freq);
    //    drawMat(
    
    //    imshow("Input Image"       , I   );    // Show the result
    //    imshow("spectrum magnitude", magI);

}

//--------------------------------------------------------------
void testApp::draw(){
//    image.draw(0, 0);
//    freq.draw(0, 0);
    
    
   
    drawMat(magI,0,0,cameraSize.width,cameraSize.height);
    image.draw(cameraSize.width,0, cameraSize.width/2,cameraSize.height/2);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
