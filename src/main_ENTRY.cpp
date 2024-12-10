

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
  const dReal density = 1.0;
  const dReal radius = 0.3;
  const double restitution = 0.9;
  const double damping = 0.1;
  const dReal starting_height = 10.0;
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

  dInitODE2(0);

  dWorldID world = dWorldCreate();
  dWorldSetGravity(world, 0.0, gravity_y, 0.0);

  dSpaceID space = dSimpleSpaceCreate(0);
  dJointGroupID collision_contact_group = dJointGroupCreate(0);
  CollisionData collision_data {world, collision_contact_group};


  //
  // Create the sphere object
  //


  // Rigid body for dynamics
  dBodyID sphere = dBodyCreate(world);
  dBodySetPosition(sphere, 0.0, starting_height, 0.0);

  // Mass
  dMass sphere_mass;
  dMassSetSphere(&sphere_mass, density, radius);
  dBodySetMass(sphere, &sphere_mass);

  // Geometry for collisions
  dGeomID sphere_geom = dCreateSphere(space, radius);
  dGeomSetBody(sphere_geom, sphere);

  //
  // Create the ground plane
  //

  dGeomID ground_geom = dCreatePlane(space, 0, 1, 0, 0);


  //
  // Simulate the world for some amount of time
  //

  constexpr dReal TIME_STOP = 10;
  constexpr dReal TIME_STEP = 0.001;
  constexpr dReal OUTPUT_STEP = 0.05;

  std::cout << "Time \"Height (R=" << restitution << ")\"\n";
  std::cout << 0 << " " << starting_height << std::endl;

  dReal next_output_time = OUTPUT_STEP;
  for (dReal time = 0.0; time < TIME_STOP + TIME_STEP/2.0; time += TIME_STEP) {

    dSpaceCollide(space, &collision_data, &handle_collisions);
    dWorldStep(world, static_cast<dReal>(TIME_STEP));
    dJointGroupEmpty(collision_contact_group);

    if (time > next_output_time) {
      const auto sphere_position = dBodyGetPosition(sphere);
      std::cout << time << " " << sphere_position[1] << std::endl;
      next_output_time += OUTPUT_STEP;
    }
  }

  //
  // Cleanup
  //

  // TODO:AJC other destroy
  dSpaceDestroy(space);
  dWorldDestroy(world);
  dCloseODE();

  //---------------------------------------------
  //---------------------------------------------
  //---------------------------------------------
  //---------------------------------------------
  //RAYLIB CODE
  // const int screenWidth = 800;
  // const int screenHeight = 600;
  // InitWindow(screenWidth, screenHeight, "Raylib basic window");
  // SetTargetFPS(60);
  // while (!WindowShouldClose()) {
  //   BeginDrawing();
  //   ClearBackground(RAYWHITE);
  //   DrawText("It works!", 20, 20, 20, BLACK);
  //   EndDrawing();
  // }
  // CloseWindow();
  return 0;
}