#include "normal.h"
#include "genericNode.h"


void normalNode( const MPI_Comm commNodes,  const  Normal *initialStatus, const int mpiProcesses){

	//Un array con el tiempo le falta a cada elemento para terminar (Cantidad de trabajos esta implícito dentro ) (no recursos, porque confundiría)
	//int* cantDeltaTQFalta = NULL;
	//int counterWorkTask = 0;
	//2. Otro array con el tiempo inicial sorteado.
	//int* initialRafflerTime = NULL;
	//int counterRafflerTime = 0;
	//3. Número de recursos que entraron, hasta este delta T.
	//unsigned long long int input = 0;

	int inputWorktask = 0;//que estan en la entrada antes del cuerpo
	int outputWorktask = 0; //que cumplieron el dalay se se pueden ir
	int bodyResource = 0; //counterWorkTask
	
	Worktask *workTaskList;
    workTaskList = (Worktask *)malloc(sizeof(Worktask)); //nodo dummy de workstasks
    workTaskList -> next = NULL;
    
	//On the fly: Promedio de las duraciones sorteadas.
	//TODO:Sumatoria de delay / cant worktask    (la primera vez)
	//(TODO:Sumatoria de delay anterior + Sumatoria de delayde este delta T) / (cant worktask anteriores + cant worktask de este delta T)

	// Máxima duración sorteada
	//double maximun = -1; //maximo de recursos
	// Minima duración sorteada
 	//double minimun = -1; //minimo de recursos

	int msg = 0;
	do {
		MPI_Bcast( &msg ,1,MPI_INT, MASTER_ID, commNodes);
	
		switch(msg){
			case ADVANCE_PAHSE:
				//printf("%d: entrada: %d, salida %d\n", initialStatus->idNode,inputWorktask,outputWorktask);
				advancePhaseNormal( &inputWorktask,  &outputWorktask, initialStatus, commNodes, mpiProcesses, FALSE);
				//printf("%d: entrada: %d, salida %d\n", initialStatus->idNode,inputWorktask,outputWorktask);
				break;
			case ADVANCE_PAHSE_PRIMA:
				printf("%d: entrada: %d, salida %d\n", initialStatus->idNode,inputWorktask,outputWorktask);
				advancePhaseNormal( &inputWorktask,  &outputWorktask, initialStatus, commNodes, mpiProcesses, TRUE);
				printf("%d: entrada: %d, salida %d\n", initialStatus->idNode,inputWorktask,outputWorktask);
				break;
			case GENERATION_PHASE:
				generationPhaseNormalPrima(&inputWorktask, &outputWorktask, &bodyResource ,commNodes, workTaskList,  initialStatus, FALSE);
			break;
			case GENERATION_PHASE_PRIMA:
				generationPhaseNormalPrima(&inputWorktask, &outputWorktask, &bodyResource ,commNodes, workTaskList,  initialStatus, TRUE);
				break;
			case CONSUME_DT:
			case PING_REPORT:
			default:
				break;
		}
	
	} while (msg != LIVE_LOCK);
	return;
}

void advancePhaseNormal(int * inputWorktask, int* outputWorktask, const Normal *initialStatus, const MPI_Comm commNodes, const int mpiProcesses,const int isPrima){ 
	int* bufferReceiver = (int*) malloc( sizeof(int)* initialStatus->countPreceders);
    int receiverCount = 1;
    int msg;
    //inicializo los request de las llegadas de recursos
    MPI_Request* requestPreceders = (MPI_Request*) malloc( sizeof(MPI_Request)* initialStatus->countPreceders);
	MPI_Request* requestFollowers = (MPI_Request*) malloc( sizeof(MPI_Request)* initialStatus->countFollowers);
    
	if( isPrima) printf("3: reciviendo los inputs\n");
	//tomo los envios pendientes del RESOURCE SEND y los paso a la entrada
	for (int i = 0 ; i < initialStatus->countPreceders; i++){
		 MPI_Irecv( &bufferReceiver[i], receiverCount, MPI_INT,  initialStatus->preceders[i], RESOURCE_SEND, commNodes, &requestPreceders[i]);
	}
	if( isPrima)printf("3: enciando los outputs\n");
	for (int i = 0 ; i < initialStatus->countFollowers; i++){
		 MPI_Isend( outputWorktask, 1, MPI_INT,  initialStatus->followers[i], RESOURCE_SEND, commNodes, &requestFollowers[i]);
	}

    if( isPrima)printf("3: Esperando...1\n");
	//espero a que todas la operaciones allan terminado
	for (int i = 0 ; i < initialStatus->countFollowers; i++){
		MPI_Wait(&requestFollowers[i], MPI_STATUS_IGNORE);
		(*outputWorktask) = 0;
	}
	if( isPrima)printf("3: Esperando...2\n");
	for (int i = 0 ; i < initialStatus->countPreceders; i++){
		MPI_Wait(&requestPreceders[i], MPI_STATUS_IGNORE);
		(*inputWorktask) += bufferReceiver[i];
	}
	
	if( !isPrima ){
		MPI_Barrier( commNodes );
		
	} else {
		int * nodesStatus = NULL;
		msg = (*inputWorktask)? FALSE: TRUE;
		printf("me quede en la barrera3\n");
		MPI_Gather(&msg, 1, MPI_INT,  nodesStatus, 1 , MPI_INT,  MASTER_ID, commNodes);
		printf(" sale de la barrera3\n");
	}
	
	free(bufferReceiver);
	free(requestPreceders);
	free(requestFollowers);
	return;
}

void generationPhaseNormalPrima(int* inputWorktask, int* outputWorktask, int* bodyResource ,const MPI_Comm commNodes, Worktask *workTaskList,  const Normal *initialStatus, const int isPrima){
	//si es deterministico 0
	if( isPrima && initialStatus->delay.distribution == DIST_DETERMINISTIC && ((int)initialStatus->delay.constant) == 0){	
		//printf("cte en los cadod de det 0  = %d", ((int)initialStatus->delay.constant) * TIME_TO_DELTA_T);
		(*outputWorktask) += (*inputWorktask);
		(*inputWorktask) = 0;
	} else {
		switch(initialStatus->delay.distribution){	
			case DIST_DETERMINISTIC:
				for(int i = 0; i < (*inputWorktask); i++){
					//printf("cte = %d", ((int)initialStatus->delay.constant) * TIME_TO_DELTA_T);
					insertWorktask(workTaskList, ((int)initialStatus->delay.constant) * TIME_TO_DELTA_T);
				}
			break;
			case DIST_UNIFORM:

			break;
			case DIST_NORMAL:

			break;
			case DIST_EXPONENTIAL:

			break;
			case DIST_TRIANGULAR:

			break;
			case DIST_BETA:

			break;
			case DIST_LOG_NORMAL:

			break;
		}

		//insertWorktask(workTaskList, unsigned long long int currentDelay,  unsigned long long int  initialDelay);
		
		(*bodyResource) += (*inputWorktask);
		(*inputWorktask) = 0;
		
	}

	//insertWorktask(workTaskList, unsigned long long int currentDelay,  unsigned long long int  initialDelay);
	//printf("espero en barrera");
	MPI_Barrier( commNodes );
	//printf("Salgo barrera en barrera");
}
