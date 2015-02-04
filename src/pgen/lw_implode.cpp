//======================================================================================
// Athena++ astrophysical MHD code
// Copyright (C) 2014 James M. Stone  <jmstone@princeton.edu>
//
// This program is free software: you can redistribute and/or modify it under the terms
// of the GNU General Public License (GPL) as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
// PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of GNU GPL in the file LICENSE included in the code
// distribution.  If not see <http://www.gnu.org/licenses/>.
//======================================================================================

// Primary header
#include "../mesh.hpp"

// Athena headers
#include "../athena.hpp"           // enums, Real
#include "../athena_arrays.hpp"    // AthenaArray
#include "../parameter_input.hpp"  // ParameterInput
#include "../fluid/fluid.hpp"      // Fluid
#include "../fluid/eos/eos.hpp"    // EOS

//======================================================================================
//! \file lw_implode.cpp
//  \brief Problem generator for square implosion problem
//
// REFERENCE: R. Liska & B. Wendroff, SIAM J. Sci. Comput., 25, 995 (2003)    */
//======================================================================================

//======================================================================================
//! \fn ProblemGenerator
//  \brief Liska & Wendroff implosion test problem generator
//======================================================================================

void Mesh::ProblemGenerator(Fluid *pfl, Field *pfd, ParameterInput *pin)
{
  MeshBlock *pmb = pfl->pmy_block;
  int is = pmb->is; int js = pmb->js; int ks = pmb->ks;
  int ie = pmb->ie; int je = pmb->je; int ke = pmb->ke;

  Real d_in = pin->GetReal("problem","d_in");
  Real p_in = pin->GetReal("problem","p_in");

  Real d_out = pin->GetReal("problem","d_out");
  Real p_out = pin->GetReal("problem","p_out");

  Real gm1 = (pfl->pf_eos->GetGamma() - 1.0);
  Real y0 = 0.5*(pmb->pmy_mesh->mesh_size.x2max + pmb->pmy_mesh->mesh_size.x2min);

// Set initial conditions
  for (int k=ks; k<=ke; k++) {
    for (int j=js; j<=je; j++) {
      for (int i=is; i<=ie; i++) {
	pfl->u(IM1,k,j,i) = 0.0;
	pfl->u(IM2,k,j,i) = 0.0;
	pfl->u(IM3,k,j,i) = 0.0;

	if(pmb->x2v(j) > (y0 - pmb->x1v(i))) {
	  pfl->u(IDN,k,j,i) = d_out;
	  pfl->u(IEN,k,j,i) = p_out/gm1;
	} else {
	  pfl->u(IDN,k,j,i) = d_in;
	  pfl->u(IEN,k,j,i) = p_in/gm1;
	}
      }
    }
  }

  return;
}