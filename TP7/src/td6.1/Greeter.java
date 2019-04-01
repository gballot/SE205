// Simple Hello-World program based on Akka actors.
//
// Based on the code from Akka's tutorial/example suite:
// http://doc.akka.io/docs/akka/2.3.14/java/hello-world.html
//

package se205.td6_1;

import akka.actor.UntypedActor;

public class Greeter extends UntypedActor {

  // Define messages that are sent/received by the Greeter
  public static enum Msg {
    GREET, DONE;
  }

  @Override
  public void onReceive(Object msg) {
    // Check the message
    if (msg == Msg.GREET) {
      // handle all GREET messages ...
      System.out.println("Hello World!");
      System.out.println("Working Directory = " + System.getProperty("user.dir"));

      // then send a reply message.
      getSender().tell(Msg.DONE, getSelf());
    } else {
      // signal that an unexpected message was received
      unhandled(msg);
    }
  }

}
