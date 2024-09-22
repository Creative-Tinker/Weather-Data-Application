## Test-Driven Development (TDD) Test Plan

There is a bash script ‘cli.sh’ which runs various tests cases using the ‘assert’ function thus verifying the behaviour of the program to ensure the CLI behaves under various conditions.  It tests individual components functionalities, by running the executable with different arguments only checking the exit code. If the code exits with a zero it means it was successful, and anything other than a non-zero implies failure. This script requires a UNIX like environment as that is the environment used to create it. Since it tests individual components functionality in isolation, it means it is classified as unit testing. There are nineteen test cases in total ranging from testing the user manual all the way up to queries meaning it tests everything that I have implemented thus far. Since minimal features were used using a testing framework such as Boost UTF would be unnecessary, as writing it from scratch is trivial. The nineteen tests can be executed using the instructions further down in the ReadME under the 'local replication' section.

Furthermore, within the project, there exists another unit test suite named 'wda-test,' integrated into the CMake build system through CTest functionality. It has test scenarios not just for location management but also for command line functions, and string utility functions.  This suite is generated from the 'main.cpp' file located in the test directory. The 'main.cpp' file contains various test cases organised within multiple functions. These tests are part of a custom testing framework implemented specifically for this project, and although it uses ctest to build it, it doesn’t use the framework itself in the source file. Custom testing was appropriate as it is a small project once again, and learning an entire framework is additional overhead. Instructions for how to run the bash test and the ctest test can also be found further in the README. 



##  Layered Architecture Diagram


![image](https://github.com/user-attachments/assets/4d2870af-6b6c-445d-9245-ff7ddd1cac8a)

This diagram illustrates the various layers comprising our application's architecture, delineating the specific technologies and components within each layer. Each layer interfaces with the one above or below it, collectively forming the overarching structure of the application.
The presentation layer encompasses the user interface, which is represented by the command line interface in our context. This where the user inputs commands and views the corresponding results. For weather query API requests, outcomes are presented in formatted tables, hence the inclusion of table and column classes (see Class Diagram). The logic layer houses the core functionality of our application, primarily composed of our C++ code, alongside external dependencies such as the rapidJSON and curl libraries which are both included within the CMakeLists.txt file. External components, including third-party libraries and APIs, are denoted in purple, contrasting with internal components marked in blue. Dashed arrows indicate dependencies, illustrating how both rapidJSON and curl are integrated with the codebase. RapidJSON facilitates the parsing of API data, while the curl library facilitates the creation of HTTP requests. Within the Data Access layer, databases reside alongside the API, serving as the primary source of weather data. An ideal application possesses the capability to store data in both local and cloud storage solutions.


##  Use Case Diagram
![image](https://github.com/user-attachments/assets/7e2be3fb-8d95-433c-adfe-55f7fa4e5785)

The diagram was created using previously created use case descriptions. The primary actor, represented by the stick figure labelled 'User' on the left, symbolizes the application's end user. Another actor, labelled 'API' on the right, represents the external weather API upon which the application relies for data and is the secondary actor. Although the weather API is not human, it is considered an actor because it interacts with our system. All actors are positioned outside the system boundary, depicted by the yellow rounded rectangle. The 11 identified use cases are placed within this boundary, with the system title bolded at the top. 

The 'User' actor is associated with four use cases through bolded lines, indicating direct interaction. The directional arrows connecting 'Add Location' to 'Modify Location,' 'Set Favourite Locations,' and 'Remove Location' signify that these actions depend on a location being added first. 'Search for Location' is a standalone use case, implying that users can search for locations without needing to add new ones. 

Similarly, 'Set Preferences' and 'Manage Offline Storage' are standalone use cases that do not depend on others. 'Query Weather Data' leads to 'Export Query Results' and 'Perform Visualizations,' indicating that querying is a prerequisite for exporting or visualizing data. However, querying itself depends on having at least one location added, as the application starts with zero locations by default.  




##  Sequence Diagrams


Sequence Diagrams visualize interactions between objects emphasizing time-based ordering.  Typically, each use case has a singular sequence diagram. However, only the first six use cases have their own respective diagrams as they are the most important in terms of priority. 

![image](https://github.com/user-attachments/assets/859c9a61-4b66-4100-ab26-8fb9117761e1)
The Add location sequence diagram above has the user Actor placed at the top left since it does not directly interact with the API. The 'objects' represented in the rectangular boxes include the command-line interface, the weather system application backend, and the database. It's important not to confuse these objects with class objects, as they serve different purposes. Instead, objects in this context refer to entities that perform actions or have actions performed on them. 


A dashed vertical line, called the lifeline, connects the User actor symbol to an activation box placed on the lifeline. The lifeline displays “the sequential events that occur to an object during the charted process” (UML Sequence Diagram Tutorial, n.d.) while the activation box indicates that “the object is active” (Sequence Diagram Tutorial – Complete Guide with Examples, 2022). The synchronous message symbol, represented by a lined arrow, indicates that the sender must wait for a response before continuing. In contrast, the dashed arrow represents the reply message symbol, indicating that these messages are replies to calls.
In the query location sequence diagram (see Appendix H), there are reflexive messages that serve as internal checks. These messages involve the object checking itself, such as 'check if connected to the internet' and 'apply default settings for unspecified parameters', as denoted by the arrows looping back into the activation boxes.


![image](https://github.com/user-attachments/assets/f024b9d9-9525-4b19-85e0-561bd7233480)

The search diagram above does not have the cloud local storage database because a user can search through the locally stored location list internally. The only way to interact with the locations object is through the location list which acts as the interface. Location list is not created within the diagram as it defaults to being created at the start of the application. 



## State Diagram

![image](https://github.com/user-attachments/assets/02cb9732-a527-418c-85d9-5a45659ed4fa)

State diagrams complement class diagrams in object-oriented design by illustrating how individual objects transition between different states in response to events or changes in their environment. While class diagrams show the static structure of a system, state diagrams model dynamic behaviour. Although a single class can have multiple objects, a state diagram focuses on modelling the behaviour of a singular object. 
States in a state diagram are represented by rounded-corner rectangles, each consisting of five components: the state name (bolded and underlined), entry activity, do activity, deferrable trigger, and exit activity. The entry activity denotes actions upon entering the state, while the exit activity signifies actions upon leaving it. The deferrable trigger represents an event awaiting processing when the object transitions to another state, distinct from the trigger event initiating the transition. Triggers, represented by labelled arrows between states, guide transitions, with initial and final states indicated by specific symbols. 
The query objects state diagram below shows transitions between the states awaiting query input, validating input, fetching data, processing data, presenting data, and handling no data. Additionally, there is no final state present as the system permits additional user requests after presenting data.


## Class Diagram

![image](https://github.com/user-attachments/assets/c8df34e0-b90d-4b5b-bb2d-700090bb7720)

This diagram features nine classes, each enclosed within its own rectangular box. Attributes, denoting class properties, are listed beneath the bolded title, while operations represent methods and are listed subsequently. Private attributes and operations are denoted by a negative sign, while public ones are indicated by a plus sign.

Associations serve as conceptual links between classes, representing potential connections between their instances. Each association includes a multiplicity indicator, defining the quantity of objects capable of engaging in the relationship (Bennett, et al., 2010). Associations are depicted by solid lines connecting the related classes, and multiplicity indicators represented by start symbols denote the number of instances involved. 
Phrases like 'contains' between Table and Column further elucidate the relationship between classes. However, their specific relationship is one of aggregation, signifying that the Table class contains a collection of Column instances within the columns vector and manages their lifecycle. This aggregation is depicted by an empty diamond symbol. Given that multiple instances of the Column class can be associated with the Table, a star symbol is included representing said multiplicity. A similar aggregation exists between Location List and Location, as the Location List includes multiple instances of Location.

A dependency exists between the Query class and the Location class. The Query class relies on the Location class to obtain latitude and longitude information necessary for constructing the URL to request the API. This reliance is indicated by the presence of a Location instance passed as a parameter in the Query constructor. There is no ownership or containment relationship between Location and Query, nor does Query extend or inherit from Location. 

The variant selection class is only called within the context of the query class meaning, the relationship is one of composition. The time Filter instance created within the Query class cannot exist independently, and once the Query object is destroyed the associated time filter object is also destroyed. The same relationship exists between variant selection and query, as well as weather variable and query. 

## Component Diagram

![image](https://github.com/user-attachments/assets/dcb00665-2e16-40bd-85b0-774644d133ad)

Component diagrams provide a static view of a system's implementation, akin to class diagrams but emphasizing components instead of classes. In this diagram, the components correspond to source code files found in the GitHub repository. Dependencies between components are indicated by dashed arrows, illustrating that the C++ files depend on their corresponding header files. These dependencies converge towards 'main.cpp,' which serves as the program's entry point. Notably, 'Query.h' and 'console.h' are central components, receiving dependencies from multiple other components and likely representing critical functionalities. Additionally, 'Utility.h' contains general string manipulation functions and is included across multiple classes but is not a class itself


## Local Replication

### 1) Preparation:
	
 [wda]$ mkdir build
	
 [wda]$ cd build
	
 [wda/build]$ cmake ..



### 2) Build and run (after preparing):
	
 [wda/build]$ cmake --build . --config Release
	
 [wda/build]$ ./wda
 


### 3) Test (after building):
CLI tests:
	[wda]$ test/cli.sh
Unit tests
	[wda]$ cd build
	[wda/build]$ ctest --output-on-failure



For a user manual see "wda --help".
