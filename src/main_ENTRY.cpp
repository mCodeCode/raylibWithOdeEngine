

#include "../include/raylib.h"
#include <ode/ode.h>

#include <iostream>
#include <string>

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
//ODE PHYSICS ENGINE CODE
struct CollisionData {
  dWorldID world;
  dJointGroupID contact_group;
};


void handle_collisions(void *data, dGeomID geom1, dGeomID geom2)
{
  auto collision_data = static_cast<CollisionData*>(data);

  // Get the rigid bodies associated with the geometries
  dBodyID body1 = dGeomGetBody(geom1);
  dBodyID body2 = dGeomGetBody(geom2);

  // Maximum number of contacts to create between bodies (see ODE documentation)
  const int MAX_NUM_CONTACTS = 8;
  dContact contacts[MAX_NUM_CONTACTS];

  // Add collision joints
  int numc = dCollide(geom1, geom2, MAX_NUM_CONTACTS, &contacts[0].geom, sizeof(dContact));
  for (int i = 0; i < numc; ++i) {

    contacts[i].surface.mode = dContactSoftERP | dContactSoftCFM | dContactApprox1 |
        dContactSlip1 | dContactSlip2;

    contacts[i].surface.mu = 50.0;
    contacts[i].surface.soft_erp = 0.96;
    contacts[i].surface.soft_cfm = 2.00;

    // struct dSurfaceParameters {
    //      int mode;
    //      dReal mu;
    //      dReal mu2;
    //      dReal rho;
    //      dReal rho2;
    //      dReal rhoN;
    //      dReal bounce;
    //      dReal bounce_vel;
    //      dReal soft_erp;
    //      dReal soft_cfm;
    //      dReal motion1, motion2, motionN;
    //      dReal slip1, slip2;
    // };

    dJointID contact = dJointCreateContact(collision_data->world,
        collision_data->contact_group, &contacts[i]);

    dJointAttach(contact, body1, body2);
  }
}


int main() {



  //---------------------------------------------
  //---------------------------------------------
  //---------------------------------------------
  //---------------------------------------------
  //ODE PHYSICS ENGINE CODE
  const dReal density = 3.0;
  const dReal sphereRadius = 0.3;
  const double restitution = 0.9;
  const double damping = 0.1;
  const dReal starting_height = 3.0;
  const dReal gravity_y = -9.81;
  const std::string name = {"sphere1"};

  //
  // Initialize ODE, create the world, and create the collision space
  //   This must be done first.
  //


  /*
  A world in ODE is used to simulate rigid body dynamics–the world is not at all concerned with collisions (except that they add constraints to the simulated dynamics). To allow for collisions between objects we next need to create a collision space.


  A space is analogous to the world above. It handles collisions between objects. Unlike most other physics libraries though, ODE requires you to write a callback function for handling collisions manually.

  The callback function takes an optional pointer to user defined data. For this example, I’ve used the data pointer to pass in what would otherwise need to be global data. Specifically, the collision_data object above is used to package up the world and collision_contact_group so that they can be passed to the collision handling callback. Here is the definition for the struct and callback: 
  
  struct CollisionData {
    dWorldID world;
    dJointGroupID contact_group;
  };

  Writing your own collision handler has definite benefits (you get a better understanding of collisions and you are forced to come up with your own sensible collision properties) and drawbacks (you don’t get to rely on the library to define sensible defaults).

  The collision handler is passed in all pairwise rigid bodies and they can each be handled separately. For example, you can attach user data to each collision geometry (described later) defining its physical properties. For this project, I assume that every collision is the same.

  Collisions are handled by creating temporary contact joints between bodies. These joints constrain the motion of bodies with respect to one another.

  Next up we create the sphere’s rigid body.


  Here you can see why we need to start by defining the world. We create a new rigid body and set its inertial properties with dBodySetMass.

  At this point we could jump to simulating the world, but the sphere wouldn’t collide with anything; instead it would just fall forever. So, let’s attach a collision object to the rigid body.

  And now add a ground plane to the same collision space.

  The call to dCreatePlane with the given arguments will create an x-z plane with a y value of zero.

  Now all that is left is to step through time.

  At each time step we need to handle collisions (attach collision contact joints), step the dynamic world, and then empty out all contact joints.

  Since ODE offers the C-API, it does require manual cleanup of all created objects. At minimum you should cleanup the following.


   */


  // Initialize ODE, create the world, and create the collision space
  // This must be done first.
  dInitODE2(0);

  dWorldID world = dWorldCreate();
  dWorldSetGravity(world, 0.0, gravity_y, 0.0);
  // dWorldSetGravity(world, 0.0, 0.0, 0.0);

  dSpaceID space = dSimpleSpaceCreate(0);
  dJointGroupID collision_contact_group = dJointGroupCreate(0);
  CollisionData collision_data {world, collision_contact_group};


  //
  // Create the sphere object
  //


  // Rigid body for dynamics
  dBodyID sphere = dBodyCreate(world);

  // in raylib it's <X,Y,Z>  with Y being the UP axis, and Z in and out

  // in ODE it's <X,Y,Z>  with Z being the UP axis
  
  dBodySetPosition(sphere, 0.0f, starting_height, 0.0f); //starting_height

  // Mass
  dMass sphere_mass;
  dMassSetSphere(&sphere_mass, density, sphereRadius);
  dBodySetMass(sphere, &sphere_mass);

  // Geometry for collisions
  dGeomID sphere_geom = dCreateSphere(space, sphereRadius);
  dGeomSetBody(sphere_geom, sphere);

  //
  // Create the ground plane
  //

  dGeomID ground_geom = dCreatePlane(space, 0, 1, 0, 0);

  //---------------------------------------------
  //---------------------------------------------
  //---------------------------------------------
  //---------------------------------------------
  //RAYLIB CODE
   // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1200;
    const int screenHeight = 700;

    InitWindow(screenWidth, screenHeight, "world camera and falling sphere with ODE engine");

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 1.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 0.5f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    DisableCursor();                    // Limit cursor to relative movement inside the window

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop QQQ
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update QQQ
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera, CAMERA_THIRD_PERSON);

        //
        // Simulate the world for some amount of time
        //

        constexpr dReal TIME_STEP = 0.01;

        dSpaceCollide(space, &collision_data, &handle_collisions);
        dWorldStep(world, static_cast<dReal>(TIME_STEP));
        dJointGroupEmpty(collision_contact_group);

        const auto sphere_position = dBodyGetPosition(sphere);
        std::cout << time << " ODE X axis --- raylib X axis " << sphere_position[0] << std::endl;
        std::cout << time << " ODE Y axis --- raylib Y axis " << sphere_position[1] << std::endl;
        std::cout << time << " ODE Z axis --- raylib Z axis " << sphere_position[2] << std::endl;
        std::cout << time << " ODE ? axis --- raylib ? axis " << sphere_position[3] << std::endl;


        // Draw QQQ
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                //DrawSphereWires(Vector3 centerPos, float radius, int rings, int slices, Color color);
                //draw falling sphere with raylib, use ODE math
                DrawSphereWires((Vector3){sphere_position[0], sphere_position[1], sphere_position[2]}, sphereRadius, 16, 16, LIME);

                //test sphere for screen reference axis
                DrawSphereWires((Vector3){0.0f, 0.0f, 0.0f}, 0.2f, 16, 16, RED);

                // DrawGrid(int slices, float spacing)  for center of screen reference
                DrawGrid(20, 0.5f);

                //this is the actual floor
                // DrawPlane(Vector3 centerPos, Vector2 size, Color color);
                //Vector2 size in px? meters??
                DrawPlane((Vector3){0.0f, 0.0f, 0.0f}, (Vector2){50, 50}, GOLD);

            EndMode3D();

            // DrawText("Enemy: 100 / 100", (int)cubeScreenPosition.x - MeasureText("Enemy: 100/100", 20)/2, (int)cubeScreenPosition.y, 20, BLACK);
            
            // DrawText(TextFormat("Cube position in screen space coordinates: [%i, %i]", (int)cubeScreenPosition.x, (int)cubeScreenPosition.y), 10, 10, 20, LIME);
            // DrawText("Text 2d should be always on top of the cube", 10, 40, 20, GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }





    // De-Initialization
    //--------------------------------------------------------------------------------------
    //closes the ODE data
    dSpaceDestroy(space);
    dWorldDestroy(world);
    dCloseODE();


    // Close window and OpenGL context (raylib)
    CloseWindow();        


    //--------------------------------------------------------------------------------------
  return 0;
}
