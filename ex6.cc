#include <hapi.h>

using namespace std;

sc_trace_file *tfHapi;

class top : public ProcessNetwork {
  public: top(sc_module_name name) : ProcessNetwork(name) {

    //Buffer sizes
    int bufferSize01 = 2;
    int bufferSize02 = 2; 
    int bufferSize12 = 3;
    
    HAPI::TimingInformation firingDurationActor0(
      3, //Firing duration
      SC_MS   
    );
    HAPI::TimingInformation firingDurationActor1(
      1, //Firing duration
      SC_MS   
    );
    HAPI::TimingInformation firingDurationActor2(
      1, //Firing duration
      SC_MS   
    );
    
    //Initialize FIFOś 
    CircularBuffer<void*> *fifo01 = new CircularBuffer<void*>("Fifo01", bufferSize01, 1, 1);
    CircularBuffer<void*> *fifo02 = new CircularBuffer<void*>("Fifo02", bufferSize02, 1, 1);
    CircularBuffer<void*> *fifo12 = new CircularBuffer<void*>("Fifo12", bufferSize12, 1, 1);
    
    //Initialize Actors
    DefaultProcess* act0 = new DefaultProcess("Actor0", tfHapi, firingDurationActor0);
    DefaultProcess* act1 = new DefaultProcess("Actor1", tfHapi, firingDurationActor1);
    DefaultProcess* act2 = new DefaultProcess("Actor2", tfHapi, firingDurationActor2);

    act0->addOutputPort()->bind(*fifo01); //Actor 0 -> FIFO -> Actor 1
    act0->addOutputPort()->bind(*fifo02); //Actor 0 -> FIFO -> Actor 2
    act1->addInputPort() ->bind(*fifo01); //Actor 0 -> FIFO -> Actor 1
    act1->addOutputPort()->bind(*fifo12); //Actor 1 -> FIFO -> Actor 2
    act2->addInputPort() ->bind(*fifo02); //Actor 0 -> FIFO -> Actor 2
    act2->addInputPort() ->bind(*fifo12); //Actor 1 -> FIFO -> Actor 2

    init_PN();
  };
};

int sc_main(int argc , char *argv[]) {
  sc_set_time_resolution(1, SC_MS);
	
  tfHapi = sc_create_vcd_trace_file (argv[0]); //Creates ex6.vcd

  top top1("Top1");
  sc_start(100, SC_MS); //Start simulation for 100 ns
  sc_close_vcd_trace_file(tfHapi);
  return 0;
}
