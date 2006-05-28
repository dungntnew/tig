 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
static int read_properties(FILE *pipe, int separator, int (*read)(char *, int, char *, int));
static char opt_encoding[20]	= "";
static bool opt_utf8		= TRUE;
		 * -b[NSPACES], --tab-size[=NSPACES]::
		if (!strncmp(opt, "-b", 2) ||
			if (opt[1] == 'b') {
	if (*opt_encoding && strcasecmp(opt_encoding, "UTF-8"))
		opt_utf8 = FALSE;

/**
 * ENVIRONMENT VARIABLES
 * ---------------------
 * Several options related to the interface with git can be configured
 * via environment options.
 *
 * Repository references
 * ~~~~~~~~~~~~~~~~~~~~~
 * Commits that are referenced by tags and branch heads will be marked
 * by the reference name surrounded by '[' and ']':
 *
 *	2006-03-26 19:42 Petr Baudis         | [cogito-0.17.1] Cogito 0.17.1
 *
 * If you want to filter out certain directories under `.git/refs/`, say
 * `tmp` you can do it by setting the following variable:
 *
 *	$ TIG_LS_REMOTE="git ls-remote . | sed /\/tmp\//d" tig
 *
 * Or set the variable permanently in your environment.
 *
 * TIG_LS_REMOTE::
 *	Set command for retrieving all repository references. The command
 *	should output data in the same format as git-ls-remote(1).
 **/

#define TIG_LS_REMOTE \
	"git ls-remote . 2>/dev/null"

/**
 * [[history-commands]]
 * History commands
 * ~~~~~~~~~~~~~~~~
 * It is possible to alter which commands are used for the different views.
 * If for example you prefer commits in the main view to be sorted by date
 * and only show 500 commits, use:
 *
 *	$ TIG_MAIN_CMD="git log --date-order -n500 --pretty=raw %s" tig
 *
 * Or set the variable permanently in your environment.
 *
 * Notice, how `%s` is used to specify the commit reference. There can
 * be a maximum of 5 `%s` ref specifications.
 *
 * TIG_DIFF_CMD::
 *	The command used for the diff view. By default, git show is used
 *	as a backend.
 *
 * TIG_LOG_CMD::
 *	The command used for the log view. If you prefer to have both
 *	author and committer shown in the log view be sure to pass
 *	`--pretty=fuller` to git log.
 *
 * TIG_MAIN_CMD::
 *	The command used for the main view. Note, you must always specify
 *	the option: `--pretty=raw` since the main view parser expects to
 *	read that format.
 **/

#define TIG_DIFF_CMD \
	"git show --patch-with-stat --find-copies-harder -B -C %s"

#define TIG_LOG_CMD	\
	"git log --cc --stat -n100 %s"

#define TIG_MAIN_CMD \
	"git log --topo-order --stat --pretty=raw %s"

/* ... silently ignore that the following are also exported. */

#define TIG_HELP_CMD \
	"man tig 2>/dev/null"

#define TIG_PAGER_CMD \
	""


 * For long loading views (taking over 3 seconds) the time since loading
 * started will be appended:
 *
 *	[main] 77d9e40fbcea3238015aea403e06f61542df9a31 - commit 1 of 779 (0%) 5s
	wmove(view->title, 0, view->width - 1);
static void
update_display_cursor(void)
{
	struct view *view = display[current_view];

	/* Move the cursor to the right-most column of the cursor line.
	 *
	 * XXX: This could turn out to be a bit expensive, but it ensures that
	 * the cursor does not jump around. */
	if (view->lines) {
		wmove(view->win, view->lineno - view->offset, view->width - 1);
		wrefresh(view->win);
	}
}
		if (split && !backgrounded) {
		}
	if (view->pipe && view->lines == 0) {
		for (i = 0; i < ARRAY_SIZE(views); i++) {
			view = &views[i];
				report("Stopped loading the %s view", view->name),
		/* XXX: Mark closed views by letting view->parent point to the
		 * view itself. Parents to closed view should never be
		 * followed. */
		if (view->parent &&
		    view->parent->parent != view->parent) {
			view->parent = view;
	 * but if we are scrolling a non-current view this won't properly
	 * update the view title. */
	int trimmed = 1;
	if (opt_utf8) {
		authorlen = utf8_length(commit->author, AUTHOR_COLS - 2, &col, &trimmed);
	} else {
		authorlen = strlen(commit->author);
		if (authorlen > AUTHOR_COLS - 2) {
			authorlen = AUTHOR_COLS - 2;
			trimmed = 1;
		}
	}
	update_display_cursor();
static int
read_ref(char *id, int idlen, char *name, int namelen)
{
	struct ref *ref;
	bool tag = FALSE;
	bool tag_commit = FALSE;

	/* Commits referenced by tags has "^{}" appended. */
	if (name[namelen - 1] == '}') {
		while (namelen > 0 && name[namelen] != '^')
			namelen--;
		if (namelen > 0)
			tag_commit = TRUE;
		name[namelen] = 0;
	}

	if (!strncmp(name, "refs/tags/", STRING_SIZE("refs/tags/"))) {
		if (!tag_commit)
			return OK;
		name += STRING_SIZE("refs/tags/");
		tag = TRUE;

	} else if (!strncmp(name, "refs/heads/", STRING_SIZE("refs/heads/"))) {
		name += STRING_SIZE("refs/heads/");

	} else if (!strcmp(name, "HEAD")) {
		return OK;
	}

	refs = realloc(refs, sizeof(*refs) * (refs_size + 1));
	if (!refs)
		return ERR;

	ref = &refs[refs_size++];
	ref->name = strdup(name);
	if (!ref->name)
		return ERR;

	ref->tag = tag;
	string_copy(ref->id, id);

	return OK;
}

	return read_properties(popen(cmd, "r"), '\t', read_ref);
}
static int
read_repo_config_option(char *name, int namelen, char *value, int valuelen)
{
	if (!strcmp(name, "i18n.commitencoding")) {
		string_copy(opt_encoding, value);
	}
	return OK;
}
static int
load_repo_config(void)
{
	return read_properties(popen("git repo-config --list", "r"),
			       "=", read_repo_config_option);
}
static int
read_properties(FILE *pipe, int separator,
		int (*read_property)(char *, int, char *, int))
{
	char buffer[BUFSIZ];
	char *name;
	int state = OK;
	if (!pipe)
		return ERR;
	while (state == OK && (name = fgets(buffer, sizeof(buffer), pipe))) {
		char *value = strchr(name, separator);
		int namelen;
		int valuelen;

		if (value) {
			namelen = value - name;
			*value++ = 0;
			valuelen = strlen(value);
			if (valuelen > 0) {
				valuelen--;
				value[valuelen] = 0;
			}
		} else {
			namelen = strlen(name);
			value = "";
			valuelen = 0;
		if (namelen)
			state = read_property(name, namelen, value, valuelen);
	if (state != ERR && ferror(pipe))
		state = ERR;
	return state;

	/* Load the repo config file first so options can be overwritten from
	 * the command line.  */
	if (load_repo_config() == ERR)
		die("Failed to load repo config.");

				report("Prompt interrupted by loading view, "
				       "press 'z' to stop loading views");
				request = REQ_SCREEN_UPDATE;
 * variables described in the  <<history-commands, "History commands">>
 * section.
 * - The prompt doesn't work while loading.
 *
 * - link:http://www.kernel.org/pub/software/scm/git/docs/[git(7)],
 * - link:http://www.kernel.org/pub/software/scm/cogito/docs/[cogito(7)]
 *
 * Other git repository browsers:
 *
 *  - gitk(1)
 *  - qgit(1)
 *  - gitview(1)