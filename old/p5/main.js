let maparr = [
    1,1,1,1,1,1,1,1,
    1,0,1,0,0,0,0,1,
    1,0,1,0,0,0,0,1,
    1,0,1,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,1,0,1,
    1,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,	
];

let mapX = 8;
let mapY = 8;
let mapS = 64;

let px = 256;
let py = 256;
let pdx = 0;
let pdy = 0;
let pa = 45;

let PRAY = 50;

let PLAYER_WIDTH = 5;
let PLAYER_HEIGHT = 5;
let WINDOW_HEIGHT = 512;
let WINDOW_WIDTH = 512;

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

    for (let i = 0; i < 60; i++) {

        ra = fixAngle(pa-30+i);

        fill(0, 255, 255);

        dof = 0;
        // Horizontal Lines
        if (Math.sin(degToRad(ra)) > 0.01)  { 
          ay = Math.floor(py/64) * 64 - 1; 
          yo = -64; 
          xo =  64/Math.tan(degToRad(ra)); 
          ax = px + (py - ay) / Math.tan(degToRad(ra)); 
        }
        else if (Math.sin(degToRad(ra)) < -0.01) { 
          ay = Math.floor(py/64) * 64 + 64; yo = 64; 
          xo = -64/Math.tan(degToRad(ra)); 
          ax = px + (py - ay) / Math.tan(degToRad(ra)); 
        }
        else { rx = px; ry = py; dof = 8; }
      
        
        while (dof < 8) {
            rx = ax+(xo*dof);
            ry = ay+(yo*dof);

            if (rx < WINDOW_WIDTH) line(px, py, rx, ry);

            if (rx > WINDOW_WIDTH || ry > WINDOW_HEIGHT) { break; }
          
            fill(0,0,255);
            rect(rx, ry, 5, 5);

            let mx, my;

            mx = Math.floor(rx / mapS);
            my = Math.floor(ry / mapS);

            if (mx >= mapX || my >= mapY || my < 0 || mx < 0) { break; }
            if (maparr[mx+(my*mapY)] != 0) { 
                hx = rx; hy = ry; 
                break; 
            }

            dof++;
        }

        // Vertical Lines
        dof = 0;

        if      (Math.cos(degToRad(ra)) < -0.01)  { ax = Math.floor(px/64) * 64 - 1; xo = -64; yo = 64*Math.tan(degToRad(ra));  ay = py + (px - ax) * Math.tan(degToRad(ra)); }
        else if (Math.cos(degToRad(ra)) > 0.01) { ax = Math.floor(px/64) * 64 + 64; xo = 64; yo = -64*Math.tan(degToRad(ra)); ay = py + (px - ax) * Math.tan(degToRad(ra)); }
        else                             { rx = px; ry = py; dof = 8; }
        

        while (dof < 8) {
            rx = ax+(xo*dof);
            ry = ay+(yo*dof);

            if (rx < WINDOW_WIDTH) line(px, py, rx, ry);

            if (rx > WINDOW_WIDTH || ry > WINDOW_HEIGHT) { break; }

            rect(rx, ry, 5, 5);

            let mx, my;

            mx = rx >> 6;
            my = ry >> 6;

            if (mx >= mapX || my >= mapY || my < 0 || mx < 0) { break; }
            if (maparr[mx+(my*mapY)] != 0) { 
                vx = rx; vy = ry; 
                break; }
          

            dof++;
        }

        // Distance
        distV = Math.sqrt((px-vx) * (px-vx) + (py-vy) * (py-vy));
        distH = Math.sqrt((px-hx) * (px-hx) + (py-hy) * (py-hy));
        let dist;

        fill(50, 50, 50);
        if (distV > distH) {
            dist = distH;
            fill(0, 255, 0);
            line((hx >> 6) << 6, (hy >> 6) << 6, 64, 64);


        } else {
            dist = distV;
            fill(0, 150, 0);
            rect((vx >> 6) << 6, (vy >> 6) << 6, 64, 64);
        }

        let ca=fixAngle(pa-ra); dist=dist*Math.cos(degToRad(ca));                                                    //fix fisheye 
        let lineH = (mapS * WINDOW_HEIGHT) / (dist); if(lineH > WINDOW_HEIGHT){ lineH = WINDOW_HEIGHT;} //line height and limit
        let lineOff = (WINDOW_HEIGHT / 2) - (lineH>>1);                                                     //line offset
        noStroke();
        rect((WINDOW_WIDTH * 2) - (i * (WINDOW_WIDTH / 60)), lineOff, (WINDOW_WIDTH / 60), lineH);
    }
}

function drawPlayer() {
    fill(0,255,0);
    rect(px-(PLAYER_WIDTH/2), py-(PLAYER_WIDTH/2), PLAYER_WIDTH, PLAYER_WIDTH);

    line(px, py, px+(PRAY*cos(degToRad(pa))), py-(PRAY*Math.sin(degToRad(pa))));
}

function setup() {
  createCanvas(512*2, 512);
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
  drawMap();
  drawPlayer();
  drawRays();
  
  pa += 1;
  
}