import os, sys, inspect, subprocess

import zopkio.adhoc_deployer as adhoc_deployer
import zopkio.runtime as runtime
from time import sleep

def setup_suite():
    # Set up authentication
    username = runtime.get_active_config("username")
    password = runtime.get_active_config("password")
    runtime.set_user(username, password)

    # Set up the target directories and properties
    userdir = os.path.join("/home", username)
    riaps_app_path = os.path.join(userdir, runtime.get_active_config("riaps_apps_path"))
    test_app_path = os.path.join(riaps_app_path, runtime.get_active_config('app_dir'))

    start_actor_path = "/opt/riaps/armhf/bin/"

    env = {"PATH": "/usr/local/bin/:" + start_actor_path + ":$PATH",
           "RIAPSHOME": "/usr/local/riaps",
           "RIAPSAPPS": "$HOME/riaps_apps",
           "LD_LIBRARY_PATH": "/opt/riaps/armhf/lib:"+test_app_path
           }

    #start_riaps_lang = "riaps_lang " + runtime.get_active_config('model_file')

    # Set up the sources
    model_path = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                              runtime.get_active_config('app_dir'),
                              runtime.get_active_config('model_file'))

    # Script to check discovery service
    discoCheckScript = "checkDiscoveryService.py"
    discoCheckScriptPath = "../../test_common"

    # Script to start the discovery
    discoStartScript = "startDiscovery.py"
    discoStartScriptPath = "../../test_common"

    # Script to stop the discovery
    discoStopScript = "stopDiscovery.py"
    discoStopScriptPath = "../../test_common"

    killRiapsScript = "killRiaps.py"
    killRiapsScriptPath = "../../test_common"

    # Deploy the riaps killer script
    for target in runtime.get_active_config('targets'):
        deployerId = "killer_" + target["host"]
        killscriptpath = os.path.abspath(
            os.path.join(os.path.dirname(os.path.abspath(__file__)), killRiapsScriptPath, killRiapsScript))

        killDeployer = adhoc_deployer.SSHDeployer(deployerId, {
            'executable': killscriptpath,
            'install_path': riaps_app_path,
            'env': env,
            'hostname': target["host"],
            "start_command": "python3 " + os.path.join(riaps_app_path, killRiapsScript)
        })
        runtime.set_deployer(deployerId, killDeployer)
        killDeployer.install(deployerId)

        # Deploy the discovery starter script
    for target in runtime.get_active_config('targets'):
        deployerId = "discostart_" + target["host"]
        startscriptpath = os.path.abspath(
            os.path.join(os.path.dirname(os.path.abspath(__file__)), discoStartScriptPath, discoStartScript))

        startDiscoveryDeployer = adhoc_deployer.SSHDeployer(deployerId, {
            'executable': startscriptpath,
            'install_path': riaps_app_path,
            'env': env,
            'hostname': target["host"],
            "start_command": "python3 " + os.path.join(riaps_app_path, discoStartScript)
        })
        runtime.set_deployer(deployerId, startDiscoveryDeployer)
        startDiscoveryDeployer.install(deployerId)

        # Deploy the discovery stop script
    for target in runtime.get_active_config('targets'):
        deployerId = "discostop_" + target["host"]
        stopscriptpath = os.path.abspath(
            os.path.join(os.path.dirname(os.path.abspath(__file__)), discoStopScriptPath, discoStopScript))

        stopDiscoveryDeployer = adhoc_deployer.SSHDeployer(deployerId, {
            'executable': stopscriptpath,
            'install_path': riaps_app_path,
            'env': env,
            'hostname': target["host"],
            "start_command": "python3 " + os.path.join(riaps_app_path, discoStopScript)
        })
        runtime.set_deployer(deployerId, stopDiscoveryDeployer)
        stopDiscoveryDeployer.install(deployerId)

    # Deploy the riaps-disco checker script
    for target in runtime.get_active_config('targets'):
        deployerId = "disco" + target["host"]
        checkscriptpath = os.path.abspath(
            os.path.join(os.path.dirname(os.path.abspath(__file__)), discoCheckScriptPath, discoCheckScript))

        checkDiscoDeployer = adhoc_deployer.SSHDeployer(deployerId, {
            'executable': checkscriptpath,
            'install_path': riaps_app_path,
            'env': env,
            'hostname': target["host"],
            "start_command": "python3 " + os.path.join(riaps_app_path, "checkDiscoveryService.py")
        })
        runtime.set_deployer(deployerId, checkDiscoDeployer)
        checkDiscoDeployer.install(deployerId)

    # Deploy the riaps-components/model file
    local_riaps_lang = "riaps_lang " + model_path
    local_test_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                                  runtime.get_active_config('app_dir'))
    
    subprocess.call(local_riaps_lang, shell=True)

    move_cmd = "mv " + runtime.get_active_config('app_dir') + '.json ' + local_test_dir
    subprocess.call(move_cmd, shell=True)

    local_model_json = os.path.join(local_test_dir,
                                    runtime.get_active_config('app_dir') + '.json')
    
    for target in runtime.get_active_config('targets'):
        model_deployer = adhoc_deployer.SSHDeployer(target["actor"], {
            'executable': local_model_json,
            'install_path': test_app_path,
            'hostname': target["host"],
            #'start_command': os.path.join(start_actor_path, "start_actor"),
            'start_command': "start_actor",

            #'args': [os.path.join(test_app_path, runtime.get_active_config('app_dir') + '.json'),
            #         target["actor"]],
            'env': env
            #'terminate_only': False,
            #'pid_keyword': model_path,
        })
        runtime.set_deployer(target["actor"], model_deployer)

        # Add test cases
        testcases = ["pubfirst_"+target["actor"], "subfirst_"+target["actor"]]


        for testcase in testcases:
            model_deployer.install(testcase, {
                'args': [os.path.join(test_app_path, runtime.get_active_config('app_dir') + '.json'),
                          target["actor"],
                         '--logfile="' + testcase + '.log"']})


        for component in runtime.get_active_config('components_so'):
            localPath = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                                     runtime.get_active_config('app_dir'),
                                     component)
            #localPath = os.path.join(os.path.dirname(os.path.abspath(__file__)),
            #                         runtime.get_active_config('app_dir'),
            #                         "lib",
            #                         component)

            #targetPath = target_path#os.path.join(riaps_app_path, runtime.get_active_config('app_dir'))
            component_deployer = adhoc_deployer.SSHDeployer(component, {
                'executable': localPath,
                'install_path': test_app_path,
                'hostname': target["host"],
                #'start_command': "start_actor",  #os.path.join(target_path, "start_actor"),
                #'args': [runtime.get_active_config('app_dir'),
                #         runtime.get_active_config('app_dir') + '.json',
                #         target["actor"]],
                'env': env,
                #'terminate_only': True,
                #'pid_keyword': component,
            })
            runtime.set_deployer(component, component_deployer)
            component_deployer.install(component)

    print("Deployment done.")

def reach_discovery():
    for target in runtime.get_active_config("targets"):
        deployerId = "disco" + target["host"]
        deployer = runtime.get_deployer(deployerId)
        deployer.start(deployerId, configs={"sync": True})

def setup():
    print("Start discovery service...")
    # Start discovery
    for target in runtime.get_active_config("targets"):
        deployerId = "discostart_" + target["host"]
        deployer = runtime.get_deployer(deployerId)
        deployer.start(deployerId, configs={"sync": False})
    sleep(2)

    reach_discovery()

    #print("Setup")
    # for process in server_deployer.get_processes():
    #  server_deployer.start(process.unique_id)


def teardown():
    print("Stop riaps actors...")

    # kill all the runing riaps actors
    for target in runtime.get_active_config("targets"):
        deployerId = "killer_" + target["host"]
        deployer = runtime.get_deployer(deployerId)
        deployer.start(deployerId, configs={"sync": True})

    print("Stop discovery...")
    # Stop discovery
    for target in runtime.get_active_config("targets"):
        deployerId = "discostop_" + target["host"]
        deployer = runtime.get_deployer(deployerId)
        deployer.start(deployerId, configs={"sync": True})
    sleep(10)
    print(" -- END -- ")

    # for process in client_deployer.get_processes():
    #  client_deployer.stop(process.unique_id)


def teardown_suite():
    print("Teardown suite")
    # for process in model_deployer.get_processes():
    #    model_deployer.undeploy(process.unique_id)
