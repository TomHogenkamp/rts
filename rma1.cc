#include <systemc.h>
#include <hapi.h>
#include <processor.h>
#include <sensortask.h>
#include <processortask.h>
#include <defaultprocesstypes.h>

using namespace std;

sc_trace_file *tfHapi;

class top : public ProcessNetwork {
	public:
		top(sc_module_name name) : ProcessNetwork(name) {

			/****************************************/
			/****************************************/
			// Parameters that influence the schedule:
			/****************************************/
			/****************************************/
			

			HAPI::TimingInformation fdsTask0(
				2,      // Minimum firing duration
				2,      // Maximum firing duration
				SC_NS,   // Time unit for the firing durations
				1        // Scale factor to steer random variable accuracy (can be used to steer the ammount of decimals) This parameter will not be used in this example
			);
//			fdsTask0.useRandomDistribution = false; // default is true when min and max fd vary

			HAPI::TimingInformation fdsTask1(
				4,      // Minimum firing duration
				4,      // Maximum firing duration
				SC_NS,   // Time unit for the firing durations
				1        // Scale factor to steer random variable accuracy (can be used to steer the ammount of decimals)
			);
// 			fdsTask1.useRandomDistribution = false; // default is true when min and max fd vary


// 			unsigned int seed = 17; // use a seed for reproducable results
// 			fdsTask0.seed = seed;
// 			fdsTask1.seed = seed;
// 			fdsTask2.seed = seed;



			/****************************************/
			
			int bufferSize0 = 1;
			CircularBuffer<void*> *fifo0= new CircularBuffer<void*>("Fifo0", bufferSize0, 1, 1);
			int bufferSize1 = 1;
			CircularBuffer<void*> *fifo1= new CircularBuffer<void*>("Fifo1", bufferSize1, 1, 1);

			double src0Period = 5;
			double src0FiringDuration = 0;			
			DefaultSensorTask* src0 = new DefaultSensorTask("source0", tfHapi, src0FiringDuration, SC_NS);
			src0->setPeriod(sc_time(src0Period, SC_NS));
			double src00FiringDuration = 5;		
			DefaultSensorTask* src00 = new DefaultSensorTask("source00", tfHapi, src00FiringDuration, SC_NS);
			src00->setPeriod(sc_time(src0Period, SC_NS));


			double src1Period = 9;
			double src1FiringDuration = 0;	
			double src1setInitialDelay = 0;
			DefaultSensorTask* src1 = new DefaultSensorTask("source1", tfHapi, src1FiringDuration, SC_NS);
			src1->setPeriod(sc_time(src1Period, SC_NS));
			src1->setInitialDelay(sc_time(src1setInitialDelay, SC_NS));
			double src10FiringDuration = 5;	
			DefaultSensorTask* src10 = new DefaultSensorTask("source10", tfHapi, src10FiringDuration, SC_NS);
			src10->setPeriod(sc_time(src1Period, SC_NS));
			src10->setInitialDelay(sc_time(src1setInitialDelay, SC_NS));

                        // the higher the number the higher the priority
			int priority0 = 2;
			int priority1 = 1;
			Processor* p = new Processor("proc", Processor::SCHED_FIXED_PRIORITY_PREEMPTIVE, tfHapi);
			DefaultProcessorTask* t0 = new DefaultProcessorTask("task0", tfHapi, fdsTask0, priority0, 0);
			DefaultProcessorTask* t1 = new DefaultProcessorTask("task1", tfHapi, fdsTask1, priority1, 0);

			t0->enablePrintResponseTime();
			t1->enablePrintResponseTime();

			p->addTask(t0);
			p->addTask(t1);

			src0->addOutputPort()->bind(*fifo0);
			t0->addInputPort()->bind(*fifo0);
			src1->addOutputPort()->bind(*fifo1);
			t1->addInputPort()->bind(*fifo1);

			init_PN();
		};
};

int sc_main(int argc , char *argv[]) {
	tfHapi = sc_create_vcd_trace_file (argv[0]); // creates rma.vcd

	top top1("Top1");

// 	sc_start(); // start simulation until stopped by  the user
	sc_start(100000, SC_NS); // start simulation for 100 ns
	sc_close_vcd_trace_file(tfHapi);
	return 0;
}
