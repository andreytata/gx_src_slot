import websocket, json, pdb, weakref

from pprint import pprint as pp

class GxRemote(object):
    """Instance with mutable methods set"""
    ___face___ = {}  # pseudo-methods container

    ___sess___ = None

    ___name___ = "undefined yet"

    def __getattr__(self, name):
        if name in self.___face___:
            return self.___face___[name]
        else:
            return object.__getattribute__(self, name)

class GxObj(object):
    def query_remote_interface(self, sock, path="."):
        """Is work only if socket connected to gxvm object. Each gxvm based object have
        'get_interface' method. Call this method w/o parameters return json dict object
        with methods connected to gxvm. Call 'get_interface' with argument 'name', when
        is some exists interface name, supported methods set changed. When 'fail' exist
        in returned json, as key, result contain error diagnistic data -- not interface
        data. So, if 'fail' in json, interface data not exists."""
        sock.send("get_interface")
        subj = json.loads(sock.recv())
        if 'fail' in subj:
            print(subj)
            return
        for i in subj:
            setattr(self, i, GxProc(i))
            print("+++ %r" % getattr(self, i) )

class GxProc(object):
    """Remote methamethod info"""
    def __init__(self, name, args=""):
        self.name = name
        self.args = args

    def __call__(self,*args,**kwargs):
        return print(self,args,kwargs)

class GxJob(GxRemote):
    """Remote executable object"""
    def get_state(self, path): pass        # where path is '.' - self or '.var_name' 
    def has_state(self, path, name) : pass # where name is ASCII state name

class GxPar(GxJob):
    """Remote PAR section, support add/remove jobs ( with check syntax support )
    Create PAR (sess-private-variable) with unordered job execution strategy and
    with indeterminated finalisation order. Each job in PAR variable on server's
    side send event to all self-connected PAR/SEQ sections on self-switch-final
    state. Final state is one of {"created","success","failure","invalid"}. List
    of the final states can be extended for each PAR variable. States of PAR-var
    also in set {"created","success","failure","invalid","started"}. Where last,
    "started" state, a not final state are. Is some possibility for append 'PAR'
    variables as job in other SEQ or PAR variables. Final state of each PAR var
    depend "condition" used. Is one of predefined conditions "all_success", or
    "all_created", "all_invalid", "all_final", "num_final" etc. Condition like
    "one_success" used if PAR section contain alternatives, where finished job
    contain target result, and all other parallel processes can be terminated
    right now.
    """
    par_jobs = {}


class GxSeq(GxJob):
    """Remote SEQ section, support add/remove lines ( with check syntax support ).
    Create SEQ (sess-private-variable) with ordered invocation, where next job-item
    invocated only if prev job-item entered in self "success" state. In other case,
    invocated other job-item specified before SEQ section invocated. Set of SEQ-var
    states equal to PAR states {"created","success","failure","invalid","started"}.
    reference to SEQ variable can be used as other SEQ/PAR vars item.
    SEQ sess.vv
        SEQ sess.vv.bb
           0:
           1:
           2:
           3:
        ? : vv, SEQ
        ? failure : vv.fail "last call return failure state:" vv.last.fail vv.last.name
        ? invalid : vv.restart  # reset to begin
        ? started : vv.wait
        ? success : vv.next
        0: sess.tt.step
        1: sess.bb.step
        2: sess.exit
    """
    seq_jobs = []

class GxVip(GxJob):
    """Visitor Interface Process - non blocking realtime process with remote
    and proxy abilities. Person or NPC controller with multiple PAR,SEQ and VIP
    variables. ,User  communicate
    """
    blog = list()    # actually list of strings, with 
    vars = {}
    uses = {}
    def new_par(self, path): pass
    def add_job(self, path): pass
    def new_seq(self, name): pass
    def new_obj(self, name): pass
    def del_obj(self, name): pass
    def wait_state(self, path, state_name) : pass
    def wait_final(self, path, json_query = {}) : pass


class GxMethod(object):
    """GxRemote interface's method object, on call must return dictionary object -
    json with remote call results. Not all operations can be finished immediately.
    But immediately returned invocation's result may contain 'wait' field with list
    of URL-like strings. Each URL-like string is shared name where session virtual
    mashine can find info about waited virtual machine instance. Is not realy some
    variable of sess-vm, but sess-vm can read json-info from waited sub-vm use URL
    as argument from this session, or from any other additional session. When some
    interface method return self-invocation-json-result with 'fail' or 'wait' keys,
    is not some fatal error, is only decision point. 'GxRemote' user can translate
    this 'fail' and 'wait' data to Python-side exceptions where real decision must
    be detected. Server side has no info what exactly must do in 'wait' or 'fail'
    situations, this info exists only at client side code, or in console-client's
    brain. To catch operation's finished state client side must call bloking proc
    wait-until-last-operation-is-finished, and try to read socket in loop forever.
    Also possible mode, when client do-not-send next SET command, but read states
    of the all affected sess-vm, until vm state changed to 'success', 'failure' or
    'invalid' - one from final vm-states. More modern way is create variable with
    multiple waiters inside single section, with result success_all, success_one,
    success_count, failure_one, failure_count, failure_all ... and inspect only
    this variable state. Is no-blocking way, with possibility continue remote
    calling other commands, and periodically verfy one or multiple waiters-sets
    """
    
    def __init__(self, sess, name):
        self.sess = weakref.ref(sess)
        self.name = name

    def __call__(self, *args, **kwargs):
        print("Call Gx Method %r %r" % (self.name, ( args, kwargs ) ) )
        return { "fail":"TODO: GxMethod.__call__ not implemented yet" }


class Gx(object):
    
    pref = "heap"
    
    path = "/"
 
    __exit = False

    def __init__(self, host, port = 8000 ):
        """Create webcocket and connect to new Gx-side session"""
        self.host = "ws://" + host
        self.port = port
        self.surl = self.host+ ":%d" % self.port 
        self.sock = websocket.create_connection(self.surl)
        self.serv = GxObj()
        self.serv.query_remote_interface( self.sock, "." )

    def dir(self, args=[]):
        """Return info about gx-side session's 'path',
        and all gx-side session variables and methods
        """
        result = dict()
        result["path"] = self.get_cwd()
        result["vars"] = self.get_variables()
        result["proc"] = self.get_interface()
        return result

    def get_cwd(self, args=[]):
        self.sock.send("get_cwd" + self.path )
        return json.loads( self.sock.recv() )

    def cd(self, path):
        self.sock.send("cd " + self.path + "/" + path )
        return json.loads( self.sock.recv() )

    def close(self):
        self.sock.close()

    def __del__(self):
        self.close()

    def get_interface( self, args = [] ):
        self.sock.send("get_interface")
        return json.loads( self.sock.recv() )

    def get_variables( self, args = [] ):
        self.sock.send("get_variables")
        return json.loads( self.sock.recv() )

    def exec(self):
        self.__exit = False
        while not self.__exit:
            line = input( ">> ")
            self.interprete(line)

    def exit(self, args = []):
        self.__exit = True

    def interprete(self, cmd):
        parts = cmd.split()
        meth_name = parts[0]
        args_list = parts[1:] 
        #pdb.set_trace()
        if hasattr( self, meth_name ):
            result = getattr( self, meth_name ) ( args_list ) 
            print( result )
        else:
            print("ERROR: console method '%s' not found" % meth_name)

if __name__=='__main__':

    import re

    from pprint import pprint as pp

    sess = Gx("localhost", 4680)

#    pp( sess.get_interface()                 )
#    pp( sess.get_interface(["arg0","arg2"] ) )
    pp( sess.get_variables()                 )
    pp( sess.dir()                           )
    pp( sess.dir( ["arg0","arg2"])           )
    pp( sess.cd("hello/world")               )

    #sess.exec()
    pp([i for i in dir(sess.serv) if not re.match("^__.*__$",i)])
