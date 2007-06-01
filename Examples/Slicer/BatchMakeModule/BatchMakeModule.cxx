#include <iostream>
#include <BatchMakeModuleCLP.h>
#include "bmScriptParser.h"
#include "bmProgressManagerGUI.h"
#include "itksys/Glob.hxx"
#include "itksys/SystemTools.hxx"
#include <itksys/Process.h>

namespace bm
{

// Create a progress manager for slicer
class ProgressManagerSlicer : public ProgressManager
{
public:
  ProgressManagerSlicer() 
    {
    m_RecentOutput = "";
    m_Progress = 0;
    m_NumberOfActions = 1;
    m_CurrentAction = 0;
    };

  ~ProgressManagerSlicer() {};
  virtual void SetStatus(MString status)
    {
    }

  virtual void IsRunning()
    {
    }
  virtual void AddAction(MString name)
    {
    std::cout << "<filter-start>" << std::endl;
    std::cout << " <filter-name>Executable " << std::endl;
    std::cout << m_CurrentAction << std::endl;
    std::cout << "</filter-name>" << std::endl;
    std::cout << "<filter-comment> " << m_RecentOutput.c_str() << std::endl;
    std::cout << "</filter-comment>" << std::endl;
    std::cout << "</filter-start>" << std::endl;

    m_CurrentAction++;
    m_Progress++;
    std::cout << "<filter-progress>" << std::endl;
    std::cout << m_Progress/m_NumberOfActions << std::endl;
    std::cout << "</filter-progress>" << std::endl;
    }
  virtual void FinishAction(MString output)
    {
    std::cout << "<filter-end>" << std::endl;
    std::cout << " <filter-name>Executable " << std::endl;
    std::cout << m_CurrentAction << std::endl;
    std::cout << " </filter-name>" << std::endl;
    std::cout << " <filter-time>" << std::endl;
    std::cout << " 0" << std::endl;
    std::cout << " </filter-time>" << std::endl;
    std::cout << "</filter-end>" << std::endl;
    }
  virtual void AddOutput(MString output)
    {
    }
  virtual void AddError(MString output)
    {
    }
  virtual void SetFinished(MString message)
    {
    }
  virtual void DisplayOutput(MString message)
    {
    m_RecentOutput = message.toChar();
    }
  virtual void DisplayError(MString message)
    {
    }

  void SetNumberOfActions(unsigned long actions)
    {
    m_NumberOfActions = actions;
    }

protected:

  std::string m_RecentOutput;
  int m_CurrentAction;
  float m_Progress;
  unsigned long m_NumberOfActions;
};

  } // end namespace

int main(int argc, char* argv[])
{
  PARSE_ARGS;

  // Write the script
  std::string script = "echo('Starting BatchMake Script')\n";
  char* buffer = new char[512];
  sprintf(buffer,"sequence(params1 %f %f %f)\n",parameter1Min,parameter1Max,parameter1Step);
  script += buffer;
  /*delete [] buffer;
  buffer = new char[512];
  sprintf(buffer,"sequence(params2 %f %f %f)\n",parameter2Min,parameter2Max,parameter2Step);
  script += buffer;*/
  delete [] buffer;
  buffer = new char[512];
  sprintf(buffer,"glob(inputFiles %s/%s)\n",dataDir.c_str(),dataMask.c_str());
  script += buffer;

  script += "setapp(gbapp @GaussianBlur)\n";

  // Create the dashboard
  script += "DashboardHost(http://www.insight-journal.org/batchmake)\n";
  script += "DashboardUser('Anonymous')\n";
  script += "DashboardKey('PUtmxTqNYXb1KUhm')\n";
  script += "CreateExperiment(exp 'Slicer Module' 'Gaussian Blur' 'Gaussian Blur')\n";
  script += "CreateMethod(meth exp 'Gaussian Blur' 'Gaussian Blur')\n";
  script += "AddMethodInput(cadSigma meth 'Sigma' 'float')\n";
  //script += "AddMethodInput(cadTimeStep meth 'Time Step' 'float')\n";
  script += "AddMethodInput(inputSlice meth 'Input Slice' 'png')\n";
  script += "AddMethodOutput(outputSlice meth 'Output Slice' 'png')\n";

  script += "foreach(file ${inputFiles})\n";
  script += "foreach(sigma ${params1})\n";
  //script += "foreach(timeStep ${params2})\n";
  script += "GetFilename(outputName ${file} NAME_WITHOUT_EXTENSION)\n";
  script += "GetFilename(outputExtension ${file} EXTENSION)\n";

  script += "Set(outputFilename "+outputDir+"/${outputName}-${sigma}${outputExtension})\n";

  script += "echo('Processing file: '${file})\n";
 /* script += "run(output '"+module;
  script += " --iterations 1";
  script += " --timeStep ${timeStep}";
  script += " --conductance ${conductance}";
  script += " ${file}";
  script += " ${outputFilename}";
  script += "')\n";*/

  script += "Set(inputSlice ${file}.png)\n";
  script += "ExtractSlice(${file} ${inputSlice})\n";
  script += "Set(cadSigma ${sigma})\n";
  //script += "Set(cadTimeStep ${timeStep})\n";


  script += "setappoption(gbapp.iterations.iterations 1)\n";
  script += "setappoption(gbapp.sigma.sigma ${sigma})\n";
  script += "setappoption(gbapp.inputVolume ${file})\n";
  script += "setappoption(gbapp.outputVolume ${outputFilename})\n";
  script += "run(output ${gbapp})\n";
  
  script += "Set(outputSlice "+outputDir+"/${outputName}-${sigma}.png)\n";
  script += "ExtractSlice(${outputFilename} ${outputSlice})\n";

  script += "DashboardSend(meth)\n";

  //script += "endforeach(timeStep params2)\n";
  script += "endforeach(sigma params1)\n";
  script += "endforeach(file inputFiles)\n";

  FILE* fic = fopen("C:/test.bms","wb");
  fprintf(fic,script.c_str());
  fclose(fic);

  // Create a progress manager gui
  bm::ScriptParser batchMakeParser;
  bm::ProgressManagerSlicer progressManager;
  
  // Come up with an approximation of the number of Run()
  unsigned int actions = (parameter1Max-parameter1Min)/parameter1Step;
  if(actions == 0)
    {
    actions = 1;
    }
  /*if((int)(parameter2Max-parameter2Min)/parameter2Step>0)
    {
    actions *= (int)(parameter2Max-parameter2Min)/parameter2Step ;
    }*/

  itksys::Glob glob;
  if(dataMaskRecursive)
    {
    glob.RecurseOn();
    }

  std::string convertedGlobExpression = dataDir;
  convertedGlobExpression += "/";
  convertedGlobExpression += dataMask;
  itksys::SystemTools::ConvertToUnixSlashes(convertedGlobExpression);
  glob.FindFiles(convertedGlobExpression);
  actions *= glob.GetFiles().size();

  progressManager.SetNumberOfActions((actions*2)+10);
  batchMakeParser.SetProgressManager(&progressManager);
  
  batchMakeParser.LoadWrappedApplication(BatchMakeWrappedApplication.c_str());
  batchMakeParser.SetBatchMakeBinaryPath(BatchMakeWrappedApplication.c_str());
  
  std::cout << "<filter-start>" << std::endl;
  std::cout << " <filter-name>CondorSubmit" << std::endl;
  std::cout << "</filter-name>" << std::endl;
  std::cout << "<filter-comment> " << std::endl;
  std::cout << "Submitting jobs to Condor" << std::endl;
  std::cout << "</filter-comment>" << std::endl;
  std::cout << "</filter-start>" << std::endl;

  // If we want to run the script locally
  if(runLocally)
    {
    batchMakeParser.ParseBuffer(script);
    }
  else
    {
    // Run condor watcher
    std::vector<const char*> args;
    args.push_back(condorwatcher.c_str());
    args.push_back(0);

    // Run the application
    itksysProcess* gp = itksysProcess_New();
    itksysProcess_SetCommand(gp, &*args.begin());
    itksysProcess_SetOption(gp,
                            itksysProcess_Option_Detach, 1);
    itksysProcess_Execute(gp);

    // Generate the script
    batchMakeParser.RunCondor(script,outputDir.c_str());
    }

  std::cout << "<filter-end>" << std::endl;
  std::cout << " <filter-name>CondorSubmit" << std::endl;
  std::cout << " </filter-name>" << std::endl;
  std::cout << " <filter-time>" << std::endl;
  std::cout << " 0" << std::endl;
  std::cout << " </filter-time>" << std::endl;
  std::cout << "</filter-end>" << std::endl;

  return EXIT_SUCCESS;
}
