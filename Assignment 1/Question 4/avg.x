/*
 * Masrik Dahir
 * Date: 02/20/2022
 * It defines variables type and their fixed sizes 
 * 
 *
 * Usage Guide (First Terminal- Server)
 * (1) make
 * (2) ./avg_svc
 * Usage Guide (Second Terminal- Client)
 * (1) ./ravg 127.0.0.1 <numbers seperated by single space>
 */


/*
 * The average procedure receives an array of real
 * numbers and returns the average of their
 * values. This toy service handles a maximum of
 * 100 numbers.
 * http://www.linuxjournal.com/article/2204?page=0,1
 */

const MAXAVGSIZE  = 100;

struct input_data 
  {
  double input_data<100>;
  };


typedef struct input_data input_data;

program AVERAGEPROG {
    version AVERAGEVERS {
        double AVERAGE(input_data) = 1;
    } = 1;
} = 24224;
