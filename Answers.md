Answers : 

Changes for RTOS Environment:

 - Make sure that different tasks can talk to the I2C device at the same time without messing things up.
 - Allow tasks to do other stuff while waiting for I2C communication to finish.
  
Improvements to the I2C API:

 - Provide ways for programs to talk to the I2C device either while waiting or without waiting.
 - If something goes wrong with talking to the device, tell the programmer what happened so they can fix it.




