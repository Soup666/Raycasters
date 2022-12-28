let maparr = [
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,
    1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,
    1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,
    1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
];

let mapX = 16;
let mapY = 16;
let mapS = 32;


let player_speed = 1;

let PRAY = 60;

let PLAYER_WIDTH = 5;
let PLAYER_HEIGHT = 5;
let WINDOW_HEIGHT = 640;
let WINDOW_WIDTH = 480;
let RAYCOUNT = 30;
let DOF = 12;

let px = 100;
let py = WINDOW_HEIGHT/2;
let pdx = 0;
let pdy = 0;
let pa = 45;

let c;

function degToRad(angle) {
    return angle * (Math.PI/180);
}

function radToDeg(angle) {
    return angle * (180/Math.PI);
}

function fixAngle(angle) {
    if (angle > 359) {
        angle -= 360;
    }
    if (angle < 0) {
        angle += 360;
    }
    return angle;
}

function drawRays() {
    let ra, xo, yo, ax, ay, dof, vx, vy, hx, hy, distV, distH, rx, ry;

    fill(0, 0, 255);

    vx = 0;
    vy = 0;
    hx = 0;
    hy = 0;
    rx = 0;
    ry = 0;
    distV = 0;
    distH = 0;

    // ra = pa-30;
    dof = 0;
  ra=fixAngle(pa+30);
  let rayCount = 0;

    for (let i = 0; i < RAYCOUNT; i++) {
       //---Vertical--- 
        dof=0; side=0; disV=100000;
        let Tan=tan(degToRad(ra));

        if     (cos(degToRad(ra))> 0.001){ rx=((px>>5)<<5)+mapS;      ry=(px-rx)*Tan+py; xo= mapS; yo=-xo*Tan;} //looking left
        else if(cos(degToRad(ra))<-0.001){ rx=((px>>5)<<5) -0.0001; ry=(px-rx)*Tan+py; xo=-mapS; yo=-xo*Tan;} //looking right
        else {                             rx=px;                        ry=py; dof=DOF;}                          //looking up or down. no hit  

        while(dof<DOF) 
        { 
            mx=(int)(rx)>>5; my=(int)(ry)>>5; mp=my*mapX+mx;                     
            if(mp>0 && mp<mapX*mapY && maparr[mp]==1){ dof=DOF; disV=cos(degToRad(ra))*(rx-px)-sin(degToRad(ra))*(ry-py);} //hit         
            else{ rx+=xo; ry+=yo; dof+=1;}                                                                            //check next horizontal
        } 
        vx=rx; vy=ry;

        //---Horizontal---
        dof=0; disH=100000;
        Tan=1.0/Tan; 

        if     (sin(degToRad(ra))> 0.001){ ry=((py>>5)<<5) -0.0001; rx=(py-ry)*Tan+px; yo=-mapS; xo=-yo*Tan;} //looking up 
        else if(sin(degToRad(ra))<-0.001){ ry=((py>>5)<<5)+mapS;      rx=(py-ry)*Tan+px; yo= mapS; xo=-yo*Tan;} //looking down
        else{ rx=px; ry=py; dof=DOF;}                                                                              //looking straight left or right
        
        while(dof<DOF) 
        { 
            mx=(int)(rx)>>5; my=(int)(ry)>>5; mp=my*mapX+mx;                          
            if(mp>0 && mp<mapX*mapY && maparr[mp]==1){ dof=DOF; disH=cos(degToRad(ra))*(rx-px)-sin(degToRad(ra))*(ry-py);} //hit         
            else{ rx+=xo; ry+=yo; dof+=1;}                                                                            //check next horizontal
        } 
        
        let pixelX;
        let textureSize = 16;
        let wallCol = [0,200,0,255];

        if(disV<disH){ // Looking at Vertical
            rx=vx; 
            ry=vy; 
            disH=disV; 
            
            pixelX = ry % 64;
        }   //horizontal hit first
        else {
            pixelX = rx % 64;
        }
        
        let normalizedPixelX = (pixelX) / (64);
        strokeWeight(1);
        if (rx < WINDOW_WIDTH && ry < WINDOW_HEIGHT)line(px, py, rx, ry);                                              //draw 2D ray
            
        let ca=fixAngle(pa-ra); disH=disH*cos(degToRad(ca));                                                    //fix fisheye 
        let lineH = (mapS * WINDOW_HEIGHT) / (disH); if(lineH > WINDOW_HEIGHT){ lineH = WINDOW_HEIGHT;} //line height and limit
        let lineOff = (WINDOW_HEIGHT / 2) - (lineH>>1);                                                     //line offset
        
        for (let y = 0.0; y <= lineH+1; y += (lineH / textureSize)) {
            let brightness = y; if (brightness > 255) brightness = 255;
            fill(wallCol[0], brightness,wallCol[2]);
            if (normalizedPixelX < 0.1) {
                fill(0, 25 * (brightness / 255), 0);
            }
            if (y > lineH-(lineH / textureSize)) {
                fill(0, 0, 0);
            }
          strokeWeight(0);
            rect( 
                (WINDOW_WIDTH) + ((rayCount+1) * ((WINDOW_WIDTH*2) / RAYCOUNT)), 
                lineOff + y + ((player_speed-1)*2*sin(frameCount)), 
                ((WINDOW_WIDTH*2) / (RAYCOUNT)), 
                (lineH / textureSize) + 1
            );
        }
        rayCount++;
        ra=fixAngle(ra-(60.0 / RAYCOUNT)); //go to next ray 
    }

}

function drawPlayer() {
    fill(0,255,0);
    rect(px-(PLAYER_WIDTH/2), py-(PLAYER_WIDTH/2), PLAYER_WIDTH, PLAYER_WIDTH);

    line(px, py, px+(PRAY*cos(degToRad(pa))), py-(PRAY*Math.sin(degToRad(pa))));
}

function setup() {
  createCanvas(512*3, 512);
}

function drawMap() {
  for(let y = 0; y < mapY; y++) {
        for(let x = 0; x < mapX; x++) {
            if (maparr[y * mapX + x] == 1){ 
                fill(color(255,0,0));
            } 
            else
            { 
                fill(color(0,255,0));
            }

            xo = x * mapS; yo = y * mapS;
          
            rect(xo+1, yo+1, mapS, mapS);

        } 
    }
}

function draw() {
  background(220);
  strokeWeight(1);
  drawMap();
  drawPlayer();
  drawRays();
  
  pa += 1;
  
}