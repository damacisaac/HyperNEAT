#include "HCUBE_Defines.h"

#include "HCUBE_ExperimentRun.h"

#include "Experiments/HCUBE_Experiment.h"

#include "Experiments/HCUBE_XorExperiment.h"
#include "Experiments/HCUBE_AtariExperiment.h"
#include "Experiments/HCUBE_AtariNoGeomExperiment.h"
#include "Experiments/HCUBE_AtariFTNeatExperiment.h"
#include "Experiments/HCUBE_AtariIntrinsicExperiment.h"
#include "Experiments/HCUBE_AtariPixelExperiment.h"
#include "Experiments/HCUBE_AtariNoiseExperiment.h"
#ifdef EPLEX_INTERNAL
#include "Experiments/HCUBE_XorCoExperiment.h"
#include "Experiments/HCUBE_SimpleImageExperiment.h"
#include "Experiments/HCUBE_RobotArmExperiment.h"
#include "Experiments/HCUBE_RobotArmLatticeExperiment.h"
#endif
#include "Experiments/HCUBE_TicTacToeExperiment.h"
#include "Experiments/HCUBE_TicTacToeGameExperiment.h"
#include "Experiments/HCUBE_TicTacToeGameNoGeomExperiment.h"
#include "Experiments/HCUBE_FindPointExperiment.h"
#include "Experiments/HCUBE_FindClusterExperiment.h"
#include "Experiments/HCUBE_FindClusterNoGeomExperiment.h"
#include "Experiments/HCUBE_CheckersExperiment.h"
#ifdef EPLEX_INTERNAL
#include "Experiments/HCUBE_SpatialExperiment.h"
#include "Experiments/HCUBE_CheckersScalingExperiment.h"
#endif
#include "Experiments/HCUBE_CheckersExperimentNoGeom.h"
#ifdef EPLEX_INTERNAL
#include "Experiments/HCUBE_CoCheckersExperiment.h"
#include "Experiments/HCUBE_CheckersExperimentFogel.h"
#endif
#include "Experiments/HCUBE_CheckersExperimentOriginalFogel.h"
#ifdef EPLEX_INTERNAL
#include "Experiments/HCUBE_CheckersExperimentPruning.h"
#include "Experiments/HCUBE_OthelloExperiment.h"
#include "Experiments/HCUBE_OthelloCoExperiment.h"
#include "Experiments/HCUBE_SpatialCasinoExperiment.h"
#include "Experiments/HCUBE_ImageCompressionExperiment.h"
#include "Experiments/HCUBE_BinaryCompressionExperiment.h"
#include "Experiments/HCUBE_FindClusterBPExperiment.h"
//#include "Experiments/HCUBE_GoExperiment.h"
#endif
#include "Experiments/HCUBE_CheckersExperimentSubstrateGeom.h"

#ifndef HCUBE_NOGUI
#include "HCUBE_MainFrame.h"
#include "HCUBE_UserEvaluationFrame.h"
#include "HCUBE_ExperimentPanel.h"
#endif

#include "HCUBE_EvaluationSet.h"

#include <boost/lexical_cast.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

namespace HCUBE
{
    ExperimentRun::ExperimentRun()
        :
        running(false),
        started(false),
        cleanup(false),
        populationMutex(new mutex()),
        frame(NULL)
    {
        //cout << "Creating experiment run" << endl;
    }

    ExperimentRun::~ExperimentRun()
    {
        delete populationMutex;
    }

    void ExperimentRun::setupExperiment(
        int _experimentType,
        string _outputFileName
        )
    {
        experimentType = _experimentType;
        outputFileName = _outputFileName;

        cout << "SETTING UP EXPERIMENT TYPE: " << experimentType << endl;

        for(int a=0;a<NUM_THREADS;a++)
        {
            switch (experimentType)
            {
#ifdef EPLEX_INTERNAL
            case EXPERIMENT_SIMPLE_IMAGE:
                experiments.push_back(shared_ptr<Experiment>(new SimpleImageExperiment("",a)));
                break;
#endif
            case EXPERIMENT_TIC_TAC_TOE:
                experiments.push_back(shared_ptr<Experiment>(new TicTacToeExperiment("",a)));
                break;
            case EXPERIMENT_TIC_TAC_TOE_GAME:
                experiments.push_back(shared_ptr<Experiment>(new TicTacToeGameExperiment("",a)));
                break;
            case EXPERIMENT_TIC_TAC_TOE_NO_GEOM_GAME:
                experiments.push_back(shared_ptr<Experiment>(new TicTacToeGameNoGeomExperiment("",a)));
                break;
#ifdef EPLEX_INTERNAL
            case EXPERIMENT_ROBOT_ARM:
                experiments.push_back(shared_ptr<Experiment>(new RobotArmExperiment("",a)));
                break;
            case EXPERIMENT_ROBOT_ARM_LATTICE:
                experiments.push_back(shared_ptr<Experiment>(new RobotArmLatticeExperiment("",a)));
                break;
#endif
            case EXPERIMENT_XOR:
                experiments.push_back(shared_ptr<Experiment>(new XorExperiment("",a)));
                break;
            case EXPERIMENT_ATARI:
                experiments.push_back(shared_ptr<Experiment>(new AtariExperiment("",a)));
                break;
            case EXPERIMENT_ATARI_NO_GEOM:
                experiments.push_back(shared_ptr<Experiment>(new AtariNoGeomExperiment("",a)));
                break;
            case EXPERIMENT_ATARI_FT_NEAT:
                experiments.push_back(shared_ptr<Experiment>(new AtariFTNeatExperiment("",a)));
                break;
            case EXPERIMENT_ATARI_HYBRID:
                experiments.push_back(shared_ptr<Experiment>(new AtariExperiment("HYBRID",a)));
                experiments.push_back(shared_ptr<Experiment>(new AtariFTNeatExperiment("HYBRID",a)));
                break;
            case EXPERIMENT_ATARI_INTRINSIC:
                experiments.push_back(shared_ptr<Experiment>(new AtariIntrinsicExperiment("",a)));
                break;
            case EXPERIMENT_ATARI_PIXEL:
                experiments.push_back(shared_ptr<Experiment>(new AtariPixelExperiment("",a)));
                break;
            case EXPERIMENT_ATARI_NOISE:
                experiments.push_back(shared_ptr<Experiment>(new AtariNoiseExperiment("",a)));
                break;
#ifdef EPLEX_INTERNAL
            case EXPERIMENT_COXOR:
                experiments.push_back(shared_ptr<Experiment>(new XorCoExperiment("",a)));
                break;
            case EXPERIMENT_MAKE_MAZE:
                //experimentRun.setExperiment(new MakeMazeExperiment());
                break;
            case EXPERIMENT_ROOM_GENERATOR:
                //experimentRun.setExperiment(new RoomGeneratorExperiment());
                break;
            case EXPERIMENT_CONNECTIVITY_GRAPH:
                //experimentRun.setExperiment(new ConnectivityGraphExperiment(4,4));
                break;
#endif
            case EXPERIMENT_FIND_POINT_EXPERIMENT:
                experiments.push_back(shared_ptr<Experiment>(new FindPointExperiment("",a)));
                break;
            case EXPERIMENT_FIND_CLUSTER_EXPERIMENT:
                experiments.push_back(shared_ptr<Experiment>(new FindClusterExperiment("",a)));
                break;
            case EXPERIMENT_FIND_CLUSTER_NO_GEOM_EXPERIMENT:
                experiments.push_back(shared_ptr<Experiment>(new FindClusterNoGeomExperiment("",a)));
                break;
#ifdef EPLEX_INTERNAL
            case EXPERIMENT_SPATIAL_CPPN:
                experiments.push_back(shared_ptr<Experiment>(new SpatialExperiment("",a)));
                break;
#endif
            case EXPERIMENT_CHECKERS:
                experiments.push_back(shared_ptr<Experiment>(new CheckersExperiment("",a)));
                break;
            case EXPERIMENT_CHECKERS_NO_GEOM:
                experiments.push_back(shared_ptr<Experiment>(new CheckersExperimentNoGeom("",a)));
                break;
#ifdef EPLEX_INTERNAL
            case EXPERIMENT_SCALABLE_CHECKERS:
                experiments.push_back(shared_ptr<Experiment>(new CheckersScalingExperiment("",a)));
                break;
            case EXPERIMENT_CHECKERS_FOGEL:
                experiments.push_back(shared_ptr<Experiment>(new CheckersExperimentFogel("",a)));
                break;
#endif
            case EXPERIMENT_CHECKERS_ORIGINAL_FOGEL:
                experiments.push_back(shared_ptr<Experiment>(new CheckersExperimentOriginalFogel("",a)));
                break;
#ifdef EPLEX_INTERNAL
            case EXPERIMENT_CHECKERS_PRUNING:
                experiments.push_back(shared_ptr<Experiment>(new CheckersExperimentPruning("",a)));
                break;
            case EXPERIMENT_COCHECKERS:
                experiments.push_back(shared_ptr<Experiment>(new CoCheckersExperiment("",a)));
                break;
            case EXPERIMENT_OTHELLO:
                experiments.push_back(shared_ptr<Experiment>(new OthelloExperiment("",a)));
                break;
            case EXPERIMENT_OTHELLO_CO:
                experiments.push_back(shared_ptr<Experiment>(new OthelloCoExperiment("",a)));
                break;
            case EXPERIMENT_IMAGE_COMPRESSION:
                experiments.push_back(shared_ptr<Experiment>(new ImageCompressionExperiment("",a)));
                break;
            case EXPERIMENT_BINARY_COMPRESSION:
                experiments.push_back(shared_ptr<Experiment>(new BinaryCompressionExperiment("",a)));
                break;
            case EXPERIMENT_SPATIAL_CASINO:
                experiments.push_back(shared_ptr<Experiment>(new SpatialCasinoExperiment("",a)));
                break;
            case EXPERIMENT_FIND_CLUSTER_BP:
                experiments.push_back(shared_ptr<Experiment>(new FindClusterBPExperiment("",a)));
                break;
                // case EXPERIMENT_GO:
                //   experiments.push_back(shared_ptr<Experiment>(new GoExperiment("",a)));
                //   break;
#endif
            case EXPERIMENT_CHECKERS_SUBSTRATE_GEOM:
                experiments.push_back(shared_ptr<Experiment>(new CheckersExperimentSubstrateGeom("",a)));
                break;
            default:
                cout << string("ERROR: Unknown Experiment Type!\n");
                throw CREATE_LOCATEDEXCEPTION_INFO("ERROR: Unknown Experiment Type!");
            }

        }

    }

    void ExperimentRun::createPopulationFromCondorRun(string populationFile,
                                                      string fitnessFunctionPrefix,
                                                      string evaluationFile,
                                                      string rom_file) {
        createPopulation(populationFile);

        allowGenerationProduction = true;

        // Check if we are running and hybrid experiment and it is time to switch over
        if (experiments[0]->getExperimentName() == "HYBRID" &&
            (!NEAT::Globals::getSingleton()->hasParameterValue("HybridConversionFinished") ||
             NEAT::Globals::getSingleton()->getParameterValue("HybridConversionFinished") != 1.0)) {
            int swapGen = int(NEAT::Globals::getSingleton()->getParameterValue("SwapGeneration")+0.001);
            int generation = (population->getGenerationCount()-1);
            if (generation == swapGen) {
                cout << "Generation " << generation << " - Hybrid switch happening now!" << endl;
                convertPopulation(rom_file);
                allowGenerationProduction = false;
            }
        }

        // Iterate all individuals to set fitness values
        vector<shared_ptr<NEAT::GeneticIndividual> >::iterator tmpIterator =
            population->getIndividualIterator(0);

        // Read the Individual fitness files and remove once read
        for (int a = 0; a < population->getIndividualCount(); a++, tmpIterator++) {
            string individualFile = fitnessFunctionPrefix + boost::lexical_cast<string>(a);
            ifstream fin(individualFile.c_str());
            if (fin.fail()) {
                cout << "Failed to read individual fitness from file " << individualFile << ". Setting fitness to 0." << endl;
                (*tmpIterator)->setFitness(0);
            } else {
                float fitness;
                fin >> fitness;
                (*tmpIterator)->setFitness(fitness);
            }
            // Delete the file
            if (remove(individualFile.c_str()) != 0) {
                perror("Error deleting file");
            }
        }

        population->adjustFitness();

        // Save the eval file
        if (!iequals(evaluationFile,"")) {
            //population->dumpBest(evaluationFile, true, true);
            savePopulationBoost(evaluationFile);
        }

        shared_ptr<NEAT::GeneticGeneration> generation = population->getGeneration();
        generation->printGenerationalStatistics();
        experiments[0]->resetGenerationData(generation);

        for (int a=0;a<population->getIndividualCount();a++) {
            experiments[0]->addGenerationData(generation,population->getIndividual(a));
        }
    }

    void ExperimentRun::createPopulation(string populationString)
    {
        if (iequals(populationString,"")) {
            allowGenerationProduction = false;
            int popSize = (int)NEAT::Globals::getSingleton()->getParameterValue("PopulationSize");
            population = shared_ptr<NEAT::GeneticPopulation>(experiments[0]->createInitialPopulation(popSize));
        } else {
#ifdef EPLEX_INTERNAL
            try {
                if (dynamic_cast<NEAT::CoEvoExperiment*>(experiments[0].get())) {
                    population = shared_ptr<NEAT::GeneticPopulation>(new NEAT::GeneticPopulation(populationString,shared_ptr<NEAT::CoEvoExperiment>((NEAT::CoEvoExperiment*)experiments[0]->clone())));
                    return;
                }
            }
            catch (const std::exception &ex) {
                throw CREATE_LOCATEDEXCEPTION_INFO(string("EXCEPTION ON DYNAMIC CAST: ")+string(ex.what()));
            }
#endif
            // Load the population
            //population = shared_ptr<NEAT::GeneticPopulation>(new NEAT::GeneticPopulation(populationString));
            loadPopulationBoost(populationString);
        }
    }

    void ExperimentRun::convertPopulation(string rom_file) {
        cout << "Converting Population: HyperNEAT --> FT-NEAT" << endl;

        NEAT::Globals::getSingleton()->setParameterValue("MutateAddNodeProbability",0.0);
        NEAT::Globals::getSingleton()->setParameterValue("MutateAddLinkProbability",0.0);
        NEAT::Globals::getSingleton()->setParameterValue("MutateDemolishLinkProbability",0.0);

        shared_ptr<AtariExperiment> hyperNEAT_experiment    =
            boost::static_pointer_cast<HCUBE::AtariExperiment>(experiments[0]);
        shared_ptr<AtariFTNeatExperiment> ftNEAT_experiment =
            boost::static_pointer_cast<HCUBE::AtariFTNeatExperiment>(experiments[1]);
        // Both experiments must be initialized to have correct ANN topology
        hyperNEAT_experiment->initializeExperiment(rom_file.c_str());
        ftNEAT_experiment->initializeExperiment(rom_file.c_str());
        population = shared_ptr<NEAT::GeneticPopulation>(
            ftNEAT_experiment->convertPopulation(population, hyperNEAT_experiment));

        cout << "Conversion Finished. Zeroing out probability of topology changes." << endl;

        if (NEAT::Globals::getSingleton()->hasParameterValue("Hybrid-FT-MutateOnlyProbability")) {
            double newValue = NEAT::Globals::getSingleton()->getParameterValue("Hybrid-FT-MutateOnlyProbability");
            NEAT::Globals::getSingleton()->setParameterValue("MutateOnlyProbability",newValue);            
            cout << "MutateOnlyProbability switched to FT-Neat value of " <<
                NEAT::Globals::getSingleton()->getParameterValue("MutateOnlyProbability") << endl;
        }

        if (NEAT::Globals::getSingleton()->hasParameterValue("Hybrid-FT-MutateLinkProbability")) {
            double newValue = NEAT::Globals::getSingleton()->getParameterValue("Hybrid-FT-MutateLinkProbability");
            NEAT::Globals::getSingleton()->setParameterValue("MutateLinkProbability",newValue);            
            cout << "MutateLinkProbability switched to FT-Neat value of " <<
                NEAT::Globals::getSingleton()->getParameterValue("MutateLinkProbability") << endl;
        }

        NEAT::Globals::getSingleton()->addParameter("HybridConversionFinished",1.0);
    }

    void ExperimentRun::loadPopulationBoost(string filename) {
        population = shared_ptr<NEAT::GeneticPopulation>(new NEAT::GeneticPopulation());
        std::ifstream ifs(filename.c_str(), std::ios::in|std::ios::binary);
        assert(ifs.good());
        boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
        in.push(boost::iostreams::gzip_decompressor());
        in.push(ifs);
        boost::archive::binary_iarchive ia(in);
        ia >> (*population);
    }

    void ExperimentRun::savePopulationBoost(string filename) {
        std::ofstream ofs(filename.c_str(),std::ios::out|std::ios::binary|std::ios::trunc);
        boost::iostreams::filtering_streambuf<boost::iostreams::output> out;
        out.push(boost::iostreams::gzip_compressor());
        out.push(ofs);
        boost::archive::binary_oarchive oa(out);
        oa << *population;
    }

    void ExperimentRun::setupExperimentInProgress(
        string populationFileName,
        string _outputFileName
        )
    {
        outputFileName = _outputFileName;

        {
            TiXmlDocument doc(populationFileName);

            bool loadStatus;

            if (iends_with(populationFileName,".gz"))
            {
                loadStatus = doc.LoadFileGZ();
            }
            else
            {
                loadStatus = doc.LoadFile();
            }

            if (!loadStatus)
            {
                throw CREATE_LOCATEDEXCEPTION_INFO("Error trying to load the XML file!");
            }

            TiXmlElement *element = doc.FirstChildElement();

            NEAT::Globals* globals = NEAT::Globals::init(element);

            //Destroy the document
        }

        int experimentType = int(NEAT::Globals::getSingleton()->getParameterValue("ExperimentType")+0.001);

        cout << "Loading Experiment: " << experimentType << endl;

        setupExperiment(experimentType,_outputFileName);

        cout << "Experiment set up.  Creating population...\n";

        createPopulation(populationFileName);

        cout << "Population Created\n";
    }

    void ExperimentRun::startCondor() {
        started = running = true;
        int generation = (population->getGenerationCount()-1);
        cout << "[HyperNEAT core] Starting experiment on generation: " << generation << endl;
        if (allowGenerationProduction) {
            produceNextGeneration();
            population->cleanupOld();
        }

        // Save the population
        //population->dumpBest(outputFileName, true, true);
        savePopulationBoost(outputFileName);

        // Try to load the population to make sure it saved correctly
        try {
            loadPopulationBoost(outputFileName);
        } catch (const std::exception &ex) {
            throw CREATE_LOCATEDEXCEPTION_INFO(string("FAILED TO LOAD NEWLY SAVED POPULATION. EXCEPTION: ")+string(ex.what()));
        }
    }


    void ExperimentRun::start()
    {
        cout << "Experiment started\n";
#ifndef _DEBUG
        try
        {
#endif
            int maxGenerations = int(NEAT::Globals::getSingleton()->getParameterValue("MaxGenerations"));

            started=running=true;

            int firstGen = (population->getGenerationCount()-1);
            for (int generations=firstGen;generations<maxGenerations;generations++)
            {
                if (generations>firstGen)
                {
                    //Even if we are loading an existing population,
                    //Re-evaluate all of the individuals
                    //as a sanity check
                    mutex::scoped_lock scoped_lock(*populationMutex);
                    cout << "PRODUCING NEXT GENERATION\n";
                    produceNextGeneration();
                    cout << "DONE PRODUCING\n";
                }

                if (experiments[0]->performUserEvaluations())
                {
#ifdef HCUBE_NOGUI
                    throw CREATE_LOCATEDEXCEPTION_INFO("ERROR: TRIED TO USE INTERACTIVE EVOLUTION WITH NO GUI!");
#else
                    frame->getUserEvaluationFrame()->updateEvaluationPanels();
                    running=false;
                    while (!running)
                    {
                        boost::xtime xt;
                        boost::xtime_get(&xt, boost::TIME_UTC);
                        xt.sec += 1;
                        boost::thread::sleep(xt); // Sleep for 1/2 second
                        //cout << "Sleeping while user evaluates!\n";
                    }
#endif
                }
                else
                {
                    while (!running)
                    {
                        boost::xtime xt;
                        boost::xtime_get(&xt, boost::TIME_UTC);
                        xt.sec += 1;
                        boost::thread::sleep(xt); // Sleep for 1/2 second
                    }
                    preprocessPopulation();
                    evaluatePopulation();
                }

                cout << "Finishing evaluations\n";
                finishEvaluations();
                cout << "Evaluations Finished\n";
            }
            cout << "Experiment finished\n";

            //cout << "Saving Dump...";
            //population->dump(outputFileName,true,false);
            //cout << "Done!\n";

            cout << "Saving best individuals...";
            string bestFileName = outputFileName.substr(0,outputFileName.length()-4)+string("_best.xml");
            population->dumpBest(bestFileName,true,true);
            cout << "Done!\n";

            cout << "Deleting backup file...";
            boost::filesystem::remove(outputFileName+string(".backup.xml.gz"));
            cout << "Done!\n";

#ifndef _DEBUG
        }
        catch (const std::exception &ex)
        {
            cout << "CAUGHT ERROR AT " << __FILE__ << " : " << __LINE__ << endl;
            CREATE_PAUSE(ex.what());
        }
        catch (...)
        {
            cout << "CAUGHT ERROR AT " << __FILE__ << " : " << __LINE__ << endl;
            CREATE_PAUSE("AN UNKNOWN EXCEPTION HAS BEEN THROWN!");
        }
#endif
    }

    void ExperimentRun::preprocessPopulation()
    {
        cout << "PREPROCESSING POPULATION\n";
        shared_ptr<NEAT::GeneticGeneration> generation = population->getGeneration();

        for(int a=0;a<generation->getIndividualCount();a++)
        {
            experiments[0]->preprocessIndividual(generation,generation->getIndividual(a));
        }
    }

    float ExperimentRun::evaluateIndividual(unsigned int individualId) {
        shared_ptr<NEAT::GeneticGeneration> generation = population->getGeneration();
        experiments[0]->preprocessIndividual(generation, generation->getIndividual(individualId));
        experiments[0]->clearGroup();
        experiments[0]->addIndividualToGroup(generation->getIndividual(individualId));
        experiments[0]->processGroup(generation);
        return generation->getIndividual(individualId)->getFitness();
    }
 
    void ExperimentRun::evaluatePopulation()
    {
        shared_ptr<NEAT::GeneticGeneration> generation = population->getGeneration();
        //Randomize population order for evaluation
        generation->randomizeIndividualOrder();

        int populationSize = population->getIndividualCount();

        if(NUM_THREADS==1)
        {
            //Bypass the threading logic for a single thread

            EvaluationSet evalSet(
                experiments[0],
                generation,
                population->getIndividualIterator(0),
                populationSize
                );
            evalSet.run();
        }
        else
        {
            int populationPerProcess = populationSize/NUM_THREADS;

            boost::thread** threads = new boost::thread*[NUM_THREADS];
            EvaluationSet** evaluationSets = new EvaluationSet*[NUM_THREADS];

            for (int i = 0; i < NUM_THREADS; ++i)
            {
                if (i+1==NUM_THREADS)
                {
                    //Fix for uneven distribution
                    int populationIteratorSize =
                        populationSize
                        - populationPerProcess*(NUM_THREADS-1);
                    evaluationSets[i] =
                        new EvaluationSet(
                            experiments[i],
                            generation,
                            population->getIndividualIterator(populationPerProcess*i),
                            populationIteratorSize
                            );
                }
                else
                {

                    evaluationSets[i] =
                        new EvaluationSet(
                            experiments[i],
                            generation,
                            population->getIndividualIterator(populationPerProcess*i),
                            populationPerProcess
                            );
                }

                threads[i] =
                    new boost::thread(
                        boost::bind(
                            &EvaluationSet::run,
                            evaluationSets[i]
                            )
                        );
            }

            //loop through each thread, making sure it is finished before we move on
            for (int i=0;i<NUM_THREADS;++i)
            {
                /*if (!evaluationSets[i]->isFinished())
                  {
                  --i;
                  boost::xtime xt;
                  boost::xtime_get(&xt, boost::TIME_UTC);
                  xt.sec += 1;
                  boost::thread::sleep(xt); // Sleep for 1/2 second
                  }*/
                threads[i]->join();
            }

            for (int i = 0; i < NUM_THREADS; ++i)
            {
                delete threads[i];
                delete evaluationSets[i];
            }

            delete[] threads;
            delete[] evaluationSets;
        }
    }

    void ExperimentRun::finishEvaluations()
    {
        cout << "Adjusting fitness...\n";
        population->adjustFitness();
        cout << "Cleaning up...\n";
        //int generationDumpModulo = int(NEAT::Globals::getSingleton()->getParameterValue("GenerationDumpModulo"));
        if (cleanup)
            population->cleanupOld(INT_MAX/2);
        cout << "Dumping best individuals...\n";
        population->dumpBest(outputFileName+string(".backup.xml"),true,true);
        //population->cleanupOld(25);
        //population->dumpBest("out/dumpBestWithGenes(backup).xml",true);

#ifndef HCUBE_NOGUI
        if (frame)
        {
            frame->updateNumGenerations(population->getGenerationCount());
        }
#endif

        cout << "Resetting generation data...\n";
        shared_ptr<NEAT::GeneticGeneration> generation = population->getGeneration();
        experiments[0]->resetGenerationData(generation);

        for (int a=0;a<population->getIndividualCount();a++)
        {
            //cout << __FILE__ << ":" << __LINE__ << endl;
            experiments[0]->addGenerationData(generation,population->getIndividual(a));
        }

        /*
          if(experimentType == EXPERIMENT_TIC_TAC_TOE_GAME)
          {
          //Take the best individual and run him a lot

          ((TicTacToeGameExperiment*)experiment)->setNumGames(20000);
          ((TicTacToeGameExperiment*)experiment)->setNumGames(100);
          }
        */
    }

    void ExperimentRun::produceNextGeneration()
    {
        cout << "Producing next generation.\n";
        try
        {
            population->produceNextGeneration();
        }
        catch (const std::exception &ex)
        {
            cout << "EXCEPTION DURING POPULATION REPRODUCTION: " << ex.what() << endl;
            exit(1);
        }
        catch (...)
        {
            cout << "Unhandled Exception\n";
        }
    }
}
