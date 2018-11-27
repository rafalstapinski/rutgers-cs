import * as React from 'react'
import { RouteComponentProps } from 'react-router'
import { Header } from '../../components/header/header'
import axios from 'axios'
import { API_HOST } from '../../../server/config'

interface MatchParams {
  transactionId: number
}

interface Props extends RouteComponentProps<MatchParams> {}

type StateProps = {
  bill: {
    transaction: {
      _id: number;
      total: number;
      timestamp: string;
      drinker_name: string;
      drinker_id: number;
      bar_id: number;
      bar_name: string;
    };
    tip: number;
    billed: Array<{
      price: number;
      product_name: string;
      is_beer: boolean;
    }>;
  };
}

export class Bill extends React.Component<Props, StateProps> {
  async componentDidMount() {
    const url = API_HOST + '/bills/' + this.props.match.params.transactionId
    const result = await axios.get(url)

    console.log(result.data.bill)

    this.setState({ bill: result.data.bill })

    console.log(this.state)
  }

  render() {
    return (
      <>
        <Header />
        {this.state &&
          this.state.bill && (
            <div>
              <p>
                {this.state.bill.transaction.drinker_name + ' at '}
                {this.state.bill.transaction.bar_name + ' at '}
                {this.state.bill.transaction.timestamp + ' for $'}
                {this.state.bill.transaction.total}
              </p>
              <div>
                <ul>
                  {this.state.bill.billed.map(i => (
                    <li>{i.product_name + ' $' + i.price}</li>
                  ))}
                </ul>
              </div>
              <div>{'Tip: $' + this.state.bill.tip}</div>
              <div />
            </div>
          )}
      </>
    )
  }
}
