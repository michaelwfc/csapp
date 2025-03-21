# 5.2 Level 3
Before you take on the Phase 5, pause to consider what you have accomplished so far. 

In Phases 2 and 3, you caused a program to execute machine code of your own design. If CTARGET had been a network server, you could have injected your own code into a distant machine.   
In Phase 4, you circumvented two of the main devices modern systems use to thwart buffer overflow attacks. Although you did not inject your own code, you were able inject a type of program that operates by stitching together sequences of existing code.
You have also gotten 95/100 points for the lab. That’s a good score. If you have other pressing obligations consider stopping right now.
Phase 5 requires you to do an ROP attack on RTARGET to invoke function touch3 with a pointer to a string representation of your cookie. That may not seem significantly more difficult than using an ROP attack to invoke touch2, except that we have made it so. 
Moreover, Phase 5 counts for only 5 points, which is not a true measure of the effort it will require. 
Think of it as more an extra credit problem for those who want to go beyond the normal expectations for the course.