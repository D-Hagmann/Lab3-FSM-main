#include <iostream>
#include <thread>
#include <chrono>
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "Vf1_fsm.h"
#include "Vbuddy.cpp"

#define MAX_SIM_CYC 1000000


int main(int argc, char **argv, char **env){
    int i;
    int tick;

    Verilated::commandArgs(argc, argv);
    
    // Init top Verilog instance
    Vf1_fsm* top = new Vf1_fsm;

    

    // Init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("f1_fsm.vcd");

    //initialise the Vbuddy
    if(vbdOpen() !=1) return(-1);
    vbdHeader("L3T2: f1_fsm");

    top->clk = 1;
    top->rst = 0;
    top->en = vbdFlag();
    

    for(i = 0; i <MAX_SIM_CYC; i++){
        for(tick = 0; tick<2; tick++){
            tfp->dump (2*i+tick);
            top->clk = !top->clk;
            top->eval ();
        }

        
        vbdBar(top->data_out & 0xFF);
        top->en = vbdFlag();
        vbdCycle(i);

        if ((Verilated::gotFinish()) || (vbdGetkey()=='f')) 
        exit(0); 

    }

    vbdClose();
    tfp->close();
    exit(0);

}

