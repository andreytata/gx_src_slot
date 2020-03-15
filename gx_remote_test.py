import sys, websocket, json, pdb, weakref

from pprint import pprint as pp

class GxRunnable(object):
    """Minimal par or seq task, state-mashine with minimal machine states set"""
    finals = { "created", "success", "failure" }
    states = { "created", "success", "failure", "started" }
    last_known_state = "created"  # not never started before, possible has no args

    def run(self):
        pass

    def invalid(self):
        pass

    def toJson(self):
        pass

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

    def ls(self, proc_expr = "^[^_][^_].*[^_][^_]$"):
        proc_re = re.compile(proc_expr)
        for i in dir(self):
            if proc_re.match(i):
                attr = getattr(self, i)
                if isinstance( attr, GxProc ):
                    print("method > %s" % i)


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
    #    self.serv.query_remote_interface( self.sock, "." )

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

    
    # def get_path(self, path):
    #     """return gx::slot's JSON info. If has no slot at 'path', in returned info
    #     field "stat" contain "unknown" as state name. (usually has no 'fail' field)"""
    #     return {'fail':'Method get_path(str) has no implementation in Gx python class'}
        
    # def set_path(self, path):
    #     """connect gx-server-side-session-instance to gx::slot at 'path'. Return JSON
    #     with "fail" field if path not exist or permission denide"""
    #     return {'fail':'Method set_path(str) has no implementation in Gx python class'}

    def send(self, **query):
        """connect gx-server-side-session-instance to gx::slot at 'path'. Return JSON
        with "fail" field if path not exist or permission denide"""
        self.sock.send(json.dumps(query))
        return json.loads(self.sock.recv())

    def query_doc():
        return """
        Each query is some call to Interface's meta-method. First query interpretation
        step  evaluated in server-side session instance. Text message decoded as JSON
        object. If some error, session object append error string template to output
        JSON 'fail' field and send back this error message to session socket.
        { 'meta' = 'get'            # method name
        , 'args' = { "re" : ".*" }  # input
        }
        """ 
        # echo = dict(query)
        # echo["fail"] = 'Method send(**query) has no implementation in Gx python class' 
        # return echo

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

    # host = "192.168.0.100"
    host = "127.0.0.1"
    port = 4680
    sess = Gx(host, port)

    # TEST 00 connect to existed GXVM use name.
    #      00.1 - get_path( path = "heap://test00" ) => JSON with path info data
    #             """Get info about path at GX side, state, type, name, exists"""
    #             GET - way to get data fields in json format, 
    #      00.2 - set_path( path = "heap://test00" ) => JSON with change path result
    #             """Sess try connect to path, as remote editor"""
    #             SET - way to change vm state and even type by sending node's data
    #                   in JSON format.

    # r = sess.get_path("heap://test00")  # read 'path' info ( at least get self access rights )
    # pp(r)
    # r = sess.set_path("heap://test00")  # connect to exists or new created VM at 'path'
    # pp(r)

    # creation step by step crete name with href to, (possible hrefs created with appropriate
    # names inside complex structure) wait all variables (par1) where finished-all-success is
    # key to enter next (seq1) block with assign each finished to all href created. After all
    # assigned, wait(par1) and (seq1) section can be deleted. After any set to name in struct
    # with hrefs, wait and assign sections must be recreated and evaluated. Is not block draw
    # and other observation processes. Is include non-blocking introspection process at node.
    r = sess.send (
        meta = "cdir",
        type = "seq",
        main = dict (
            type = "run",
            proc = "find_or_new",   # void find_or_new(const QJsonObject& i, QJsonObject& o)
            args = dict ( type = "glsl_window", path = "heap://main/glsl" ),
            success = "window_success %s" % id(sess),
            failure = "window_failure %s" % id(sess),
        ),
        on_success = dict ( type = "set", title = "Hello World", width = 800, height = 600 ),
        on_failure = dict ( type = "get", width ="", height="", is_fullscreen="", glsl_version="" ),
    )
    pp(r)

    r = sess.send (
        meta = "cdir",
        type = "glsl_window",
        path = "heap://main/glsl",
        echo = "window_finished event to %s" % id(sess),
        set = dict ( title = "Hello World", width = 800, height = 600 ),
        get = dict ( width ="", height="", is_fullscreen="", glsl_version="")
    )
    pp(r)

    pp([i for i in dir(sess.serv) if not re.match("^__.*__$",i)])
    sess.serv.ls()

# SET => Call Interface's slot "set(QJsonObject,QJsonObject&)"
    r = sess.send ( meta = 'cset', args = dict ( path = "heap://tests/test_um4f" ) )
    pp(r)

# DIR => Call Session's or Interface's slot "dir(QJsonObject,QJsonObject&)"
    r = sess.send ( meta = 'cdir', args = dict ( path = "heap://tests/test_um4f" ) )
    pp(r)

# GET => Call Session's or Interface's slot "get(QJsonObject,QJsonObject&)"

    query_template = dict( meta = 'cget', args = dict ( path = "" ))

    query_targets = [
        "heap://tests/test_qstr" , "heap://tests/test_bool" ,
        "heap://tests/test_real" , "heap://tests/test_none" ,
        "heap://tests/test_uv2f" , "heap://tests/test_uv3f" ,
        "heap://tests/test_dict" , "heap://tests/test_list" ,
        "/main"
        ]

    for path in query_targets:
        query = query_template
        query["args"]["path"] = path
        r = sess.send( **query )
        print("\n---TEST--" + path + "----")
        pp(query)
        print(">>> %s:%s echo:" % (sess.host,sess.port) )
        pp(r)

# VARS, EDIT, HREF, WREF, SELF
#     Interface's variables stored in Interface's dict VARS and has life-time
#   depended from Interface's instance life time and self own life time based
#   on VARS records:
#     EDIT(name, path) => open to edit node's value (new/del/replace)
    r = sess.send ( meta = 'edit', args = dict ( name = "edit0", path = "heap://tests/test_edit0" ) )
    pp(r)

#     HREF(name, path) => hard ref to shared object w/o some edit abilities
    r = sess.send ( meta = 'href', args = dict ( name = "href0", path = "heap://tests/test_href0" ) )
    pp(r)

#     WREF(name, path) => weak ref to shared object w/o some edit abilities
    r = sess.send ( meta = 'wref', args = dict ( name = "wref0", path = "heap://tests/test_wref0" ) )
    pp(r)

#     SELF(name)       => hard ref to not-shared self variable with full edit abilities set
    r = sess.send ( meta = 'self', args = dict ( name = "self0", path = "heap://tests/test_self0" ) )
    pp(r)


# VAR => Call Interface's Variable's Slot "get(QJsonObject,QJsonObject&)"
    # r = sess.send (
    #     meta = 'var',       # Interface's method "var(QJsonObject,QJsonObject&)"
    #     name = 'var_name',  # Interface's varible name
    #     eval = 'get',       # vars['var_name'] Slot "get(QJsonObject,QJsonObject&)"
    #     args = dict (
    #         path = "heap://tests/test_um4f",
    #         echo = "breaf"
    #     )
    # )
    # pp(r)
