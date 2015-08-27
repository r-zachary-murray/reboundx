#include <stdio.h>
#include <math.h>
#include "rebound.h"
#include "reboundx.h"
#include "gr.h"

void rebx_gr(struct reb_simulation* const sim){
	struct rebx_params_gr* rebxparams = &((struct rebx_extras*)(sim->extras))->gr;
	const double C = rebxparams->c;
	const int _N_real = sim->N - sim->N_var;
	const double G = sim->G;
	struct reb_particle* const particles = sim->particles;
	
	const struct reb_particle sun = particles[0];
	for (int i=1; i<_N_real; i++){
		const double dx = particles[i].x - sun.x;
		const double dy = particles[i].y - sun.y;
		const double dz = particles[i].z - sun.z;
		const double r2 = dx*dx + dy*dy + dz*dz;
		const double _r = sqrt(r2);
		const double dvx = particles[i].vx - sun.vx;
		const double dvy = particles[i].vy - sun.vy;
		const double dvz = particles[i].vz - sun.vz;
		// Benitez and Gallardo 2008
		const double alpha = G*sun.m/(_r*_r*_r*C*C);
		const double v2 = dvx*dvx + dvy*dvy + dvz*dvz;
		const double beta = 4.*G*sun.m/_r - v2;
		const double gamma = 4.*(dvx*dx + dvy*dy + dvz*dz);

		const double dax = alpha*(beta*dx + gamma*dvx);
		const double day = alpha*(beta*dy + gamma*dvy);
		const double daz = alpha*(beta*dz + gamma*dvz);
		const double massratio = particles[i].m/particles[0].m;

		particles[i].ax += dax;
		particles[i].ay += day;
		particles[i].az += daz;
		particles[0].ax -= massratio*dax;
		particles[0].ay -= massratio*day;
		particles[0].az -= massratio*daz;
	}
}

