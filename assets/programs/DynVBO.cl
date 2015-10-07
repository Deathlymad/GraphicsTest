__kernel void updatePosVec(__global float* Buffer, __global const float* PosChange) {
 
    // Get the index of the current element to be processed
    int i = get_global_id(0) * 7;

    //Move
    Buffer[i] = Buffer[i] + PosChange[0];
    Buffer[i + 2] = Buffer[i + 2] + PosChange[1];
	
	//Generate Heigth
	float x = Buffer[i];
	float y = Buffer[i + 2];
	
    Buffer[i + 1] = 10*sin(x/25)+6*cos(y/16)+12*sin((x+y)/30)+12*cos((x-y)/30);
}