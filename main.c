/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */
#include <mpi.h>
#include <stdio.h>
#include "scheduler.h"
#include "RNGs.h"
#include "genericNode.h"

int main(int argc, char **argv){
  int idNodo;

  /* Initialize MPI */
  MPI_Init(&argc, &argv);

  /* Find out my identity in the default communicator */
  MPI_Comm_rank(MPI_COMM_WORLD, &idNodo);
  if (idNodo == MASTER_ID) {
    scheduler();
  } else {
    genericNode(idNodo);
  }
  /* Shut down MPI */
  MPI_Finalize();
  return 0;
}
