import * as React from 'react'
import { Header } from '../../components/header/header'
import axios from 'axios'
import { API_HOST } from '../../../server/config'
import Highlight from 'react-highlight'

type Verification = {
  areAllTransactionsWithinBusinessHours: boolean;
  areAllPricesConsistent: boolean;
  doAllDrinkersFrequentBarsInSameState: boolean;
}

type StateProps = {
  verification: Verification;
  pageWidth: number;
  loading: boolean;
}

const passedStyle = {
  color: 'green'
}

const failedStyle = {
  color: 'red'
}

export class Interface extends React.Component<{}, StateProps> {
  updateWindowDimensions() {
    window.addEventListener('resize', this.updateWindowDimensions)
    this.setState({ pageWidth: window.innerWidth })
  }

  async componentDidMount() {
    this.setState({ loading: true })
    const url = API_HOST + '/verify/'
    const result = await axios.get(url)

    this.setState({ verification: result.data.verification, loading: false })
  }

  render() {
    return (
      <>
        <Header />
        <div>
          <h3>Integrity verification</h3>
          <p>(This takes several seconds)</p>
          <h3>
            Checks the price consistency pattern -
            {this.state &&
              this.state.verification &&
              !this.state.loading && (
                <span
                  style={
                    this.state.verification.areAllPricesConsistent
                      ? passedStyle
                      : failedStyle
                  }
                >
                  {this.state.verification.areAllPricesConsistent
                    ? ' Passed!'
                    : ' Failed!'}
                </span>
              )}
            {this.state && this.state.loading && <span> Loading...</span>}
          </h3>
          <Highlight className='SQL'>
            {`CREATE FUNCTION areAllPricesConsistent() RETURNS BOOLEAN BEGIN
	DECLARE i_sells INT DEFAULT 0;
	DECLARE n_sells INT;
	DECLARE t_price FLOAT;
	DECLARE t_product_id BIGINT UNSIGNED;
	DECLARE product_price FLOAT;
	DECLARE bar_id BIGINT UNSIGNED;
	
	SELECT COUNT(*) INTO n_sells FROM sells;
	
	WHILE (i_sells < n_sells) DO

			SELECT product_id, price INTO t_product_id, t_price FROM sells limit i_sells,1;
			SELECT p.price INTO product_price FROM products p WHERE p.id = t_product_id;
			
			IF (t_price > product_price + 1) THEN
				RETURN FALSE;
			END IF;
			
			SET i_sells = i_sells + 1;

	END WHILE;
	
	RETURN TRUE;
	
END;`}
          </Highlight>
          <h3>
            Checks same state pattern -
            {this.state &&
              this.state.verification &&
              !this.state.loading && (
                <span
                  style={
                    this.state.verification.doAllDrinkersFrequentBarsInSameState
                      ? passedStyle
                      : failedStyle
                  }
                >
                  {this.state.verification.doAllDrinkersFrequentBarsInSameState
                    ? ' Passed!'
                    : ' Failed!'}
                </span>
              )}
            {this.state && this.state.loading && <span> Loading...</span>}
          </h3>

          <Highlight className='SQL'>
            {`CREATE FUNCTION doAllDrinkersFrequentBarsInSameState() RETURNS BOOLEAN
	BEGIN
		DECLARE bar_state VARCHAR(30);
		DECLARE drinker_state VARCHAR(30);
		DECLARE drinker_id BIGINT UNSIGNED;
		DECLARE bar_id BIGINT UNSIGNED;
		DECLARE i_drinker INT DEFAULT 0;
		DECLARE i_bar INT DEFAULT 0;
		DECLARE n_drinker INT;
		DECLARE n_bar INT;
		
		SELECT COUNT(*) INTO n_drinker FROM drinkers;
		
		WHILE (i_drinker < n_drinker) DO
		
			SELECT state INTO drinker_state FROM drinkers LIMIT i_drinker,1;
			SELECT drinker_id INTO drinker_state FROM drinkers LIMIT i_drinker,1;
			
			SELECT COUNT(*) INTO n_bar FROM frequents f WHERE f.drinker_id = drinker_id;
		
			SET i_bar = 0;
			
			WHILE (i_bar < n_bar) DO
			
				SELECT f.bar_id INTO bar_id FROM frequents WHERE f.drinker_id = drinker_id LIMIT i_bar,1;
				
				SELECT bars.state INTO bar_state FROM bars WHERE bars.id = bar_id;
				
				IF (bar_state <> drinker_state) THEN
					RETURN false;
				END IF;
			
			END WHILE;
			
			SET i_drinker = i_drinker + 1;
		END WHILE;
		
		RETURN true;
		
	END;
`}
          </Highlight>
          <h3>
            Checks transaction time pattern -
            {this.state &&
              this.state.verification &&
              !this.state.loading && (
                <span
                  style={
                    this.state.verification
                      .areAllTransactionsWithinBusinessHours
                      ? passedStyle
                      : failedStyle
                  }
                >
                  {this.state.verification.areAllTransactionsWithinBusinessHours
                    ? ' Passed!'
                    : ' Failed!'}
                </span>
              )}
            {this.state && this.state.loading && <span> Loading...</span>}
          </h3>
          <Highlight className='SQL'>
            {`CREATE FUNCTION areAllTransactionsWithinBusinessHours() RETURNS BOOLEAN
	BEGIN
		DECLARE transaction_time TIMESTAMP;
		DECLARE ts INT;
		DECLARE t_bar_id BIGINT UNSIGNED;
		DECLARE open_time INT;
		DECLARE close_time INT;
		DECLARE i_transaction INT DEFAULT 0;
		DECLARE n_transaction INT;
		
		SELECT COUNT(*) INTO n_transaction FROM transactions;
		
		WHILE (i_transaction < n_transaction) DO
		
 			SELECT t.time, t.bar_id INTO transaction_time, t_bar_id FROM transactions t LIMIT i_transaction,1;
			SELECT opens, closes INTO open_time, close_time FROM bars WHERE bars.id = t_bar_id;
		
			SET ts = ((HOUR(transaction_time) * 60) + MINUTE(transaction_time));
			
			IF ((close_time < open_time) AND NOT (ts < open_time XOR ts > close_time)) THEN
				RETURN false;
			END IF;
			
			IF ((open_time < close_time) AND NOT (ts > open_time AND ts < close_time)) THEN
				RETURN false;
			END IF; 
			
			SET i_transaction = i_transaction + 1;
		END WHILE;
		
		RETURN true;
		
	END;`}
          </Highlight>
        </div>
      </>
    )
  }
}
