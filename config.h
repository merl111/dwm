#include <X11/XF86keysym.h>

/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Terminus:size=12" };
static const char dmenufont[]       = "Terminus:size=12";
static const char normbordercolor[] = "#f1f1f1";
static const char normbgcolor[]     = "#212121";
static const char normfgcolor[]     = "#f1f1f1";
static const char selbordercolor[]  = "#a6a6a6";
static const char selbgcolor[]      = "#212121";
//static const char selbgcolor[]      = "#a6a6a6";
static const char selfgcolor[]      = "#f1f1f1";

static const char *colors[][3]      = {
	/*               fg           bg           border   */
	[SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
	[SchemeSel] =  { selfgcolor,  selbgcolor,  selbordercolor },
};

/* tagging */
static const char *tags[] = { "1:term", "2:web", "3:dev", "4:db", "5:email", "6:vm", "7:stuff" };

static const char pp_moni[]  = "pdfpc - presenter";
static const char pp_proj[]  = "pdfpc - presentation";

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     iscentered  isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            False,      True,        -1 },
	{ NULL,       NULL,       "Scratch",  0,            True,       True,        -1 },
	{ NULL,       NULL,       "mail",     1 << 3,       False,      False,       -1 },
	{ NULL,       NULL,       "irc",      1 << 2,       False,      False,       -1 },
	{ NULL,       NULL,       "Terminal", 1,            False,      False,       -1 },
	{ NULL, "Tor Browser",    NULL,       1 << 1,       False,      False,       -1 },
	{ "Spotify",  NULL,       NULL,       1 << 5,       False,      False,       -1 },
	{ NULL,       NULL,       pp_moni,    0,            False,      True,       2  },
	{ NULL,       NULL,       pp_proj,    0,            False,      True,       1  },
};

/* layout(s) */
static const float mfact     = 0.5;  /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TK(KEY,TAG) \
	{ KeyPress, MODKEY,                         KEY,  view,        {.ui = 1 << TAG} }, \
	{ KeyPress, MODKEY|ShiftMask,               KEY,  toggleview,  {.ui = 1 << TAG} }, \
	{ KeyPress, MODKEY|ControlMask,             KEY,  tag,         {.ui = 1 << TAG} }, \
	{ KeyPress, MODKEY|ControlMask|ShiftMask,   KEY,  toggletag,   {.ui = 1 << TAG} },
#define RK(MASK,KEY,ACTION) \
	{ KeyPress, MASK,                           KEY,  ACTION,      {.i  = +1 } }, \
	{ KeyPress, MASK|ShiftMask,                 KEY,  ACTION,      {.i  = -1 } },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
#define ESHCMD(cmd) SHCMD("exec " cmd)

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-i", "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };

//static const char *mpdmenu_library[]  = { "mpdmenu", "-l", "::", "-i", "-l", "20", "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
//static const char *mpdmenu_playlist[] = { "mpdmenu", "-p", "::", "-i", "-l", "20", "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *btmenu_connect[]  = { "btmenu", "-c", "::", "-i", "-l", "20", "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *btmenu_disconnect[] = { "btmenu", "-d", "::", "-i", "-l", "20", "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *clipmenu[]         = { "clipmenu", "-i", "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *passmenu[]         = { "passmenu", "-i", "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };

static Key keys[] = {
  TK(                     XK_1,            		0               )
  TK(                     XK_2,            		1               )
  TK(                     XK_3,            		2               )
  TK(                     XK_4,            		3               )
  TK(                     XK_5,            		4               )
  TK(                     XK_6,    			   	5               )
  TK(                     XK_7,   			   	6               )
  RK( MODKEY,             XK_comma,        focusstack      )
  RK( MODKEY|ControlMask, XK_comma,        pushstack       )
  RK( MODKEY,             XK_period,       focusmon        )
  RK( MODKEY|ControlMask, XK_period,       tagmon          )

	{ KeyPress,   MODKEY,             XK_p,            spawn,          {.v = dmenucmd } },
	//{ KeyPress,   MODKEY,             XK_2,            spawn,          {.v = passmenu } },
	//{ KeyPress,   MODKEY,             XK_3,            spawn,          {.v = clipmenu } },
	//{ KeyPress,   MODKEY,             XK_4,            spawn,          {.v = btmenu_connect } },
	//{ KeyPress,   MODKEY|ControlMask, XK_4,            spawn,          {.v = btmenu_disconnect } },
	{ KeyPress,   MODKEY,             XK_Return,       spawn,          ESHCMD("st -title Terminal") },
	{ KeyPress,   MODKEY|ControlMask, XK_t,            setlayout,      {.v = &layouts[0] } },
	{ KeyPress,   MODKEY|ControlMask, XK_b,            setlayout,      {.v = &layouts[3] } },
	{ KeyPress,   MODKEY|ControlMask, XK_h,            setlayout,      {.v = &layouts[4] } },
	{ KeyRelease, MODKEY|ControlMask, XK_F10,          spawn,          ESHCMD("screenshot --focused") },
	{ KeyRelease, MODKEY,             XK_F10,          spawn,          ESHCMD("screenshot --all") },
	{ KeyRelease, False,              XK_F10,          spawn,          ESHCMD("screenshot --select") },
	{ KeyPress,   MODKEY,             XK_Delete,       spawn,          ESHCMD("gnome-screensaver-command -l") },
	{ KeyPress,   MODKEY|ShiftMask,   XK_c,      	   killclient,     {0}  },
	{ KeyPress,   MODKEY,             XK_b,      	   togglebar,      {0} },
    { KeyPress,   MODKEY,             XK_j,      	   focusstack,     {.i = +1 } },
    { KeyPress,   MODKEY,             XK_k,      	   focusstack,     {.i = -1 } },
    { KeyPress,   MODKEY,             XK_i,      	   incnmaster,     {.i = +1 } },
    { KeyPress,   MODKEY,             XK_d,      	   incnmaster,     {.i = -1 } },

	//{ KeyPress,   MODKEY,             XK_Delete,       spawn,          SHCMD("sleep 1; xset dpms force off") },
	//{ KeyPress,   MODKEY,             XK_t,            spawn,          ESHCMD("trello-popup") },
	//{ KeyPress,   MODKEY,             XK_e,            spawn,          ESHCMD("nolock '1 hour'") },
	//{ KeyPress,   MODKEY,             XK_r,            spawn,          ESHCMD("nota-todo-reminder") },
	{ KeyPress,   MODKEY,             XK_Tab,          zoom,           {0} },
	{ KeyPress,   MODKEY,             XK_BackSpace,    killclient,     {0} },
	{ KeyPress,   MODKEY|ShiftMask,   XK_BackSpace,    killunsel,      {0} },
	{ KeyPress,   MODKEY|ShiftMask,   XK_space,        togglefloating, {0} },
	{ KeyPress,   Mod1Mask|ShiftMask, XK_q,            quit,           {0} },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

/* vim: set noexpandtab: */
