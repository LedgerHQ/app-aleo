#!/usr/bin/env python3

import argparse
import json

import sys

from ledgerblue.comm import getDongle

sys.path.append("../../")
from tests.application_client.transaction import Transaction


if __name__ == "__main__":

	parser = argparse.ArgumentParser()
	parser.add_argument("--file", help="json file", default=None)
	parser.add_argument("--dry-run", help="just print apdus", action='store_true')
	args = parser.parse_args()

	if args.file is None:
		exit(-1)

	json_file = open(args.file)
	data = json.load(json_file)
	cmds = data['cmds']

	tx = Transaction()

	if not args.dry_run:
		dongle = getDongle(True)

	for cmd in cmds:
		apdus = tx.gen_apdus_tx(cmd)
		if cmd['type'] != 'get_tvk':
			print('{} : {}.{}'.format(cmd['type'], cmd['request']['program_id'], cmd['request']['function_name']))
		else:
			print('get_tvk : {:d}'.format(cmd['index']))
		for apdu in apdus:
			if args.dry_run:
				print('echo {} | python3 -m ledgerblue.runScript --apdu'.format(apdu))
			else:
				result = dongle.exchange(bytes.fromhex(apdu))
		print()
